#include "Inventory.hpp"
#include "../globals.hpp"
#include "../json_schemas/InventoryData.hpp"
#include "Player.hpp"
#include "chrono"
#include "cpr/cpr.h"
#include "string"
#include "vector"   

#define consoleLog GLOBALS::console.addOutput

Inventory::Inventory(std::string steamid) : steamid(steamid) {}

static std::chrono::time_point lastRequest = std::chrono::steady_clock::now() - std::chrono::seconds(2);

void Inventory::GetInventory() {
    consoleLog("Getting inventory for " + steamid, SEVERITY::INFO);

    std::string url = "https://steamcommunity.com/inventory/" + steamid + "/440/2?l=english&count=4000";

    JsonInventory::Inventory inventoryData;

    std::chrono::steady_clock::time_point startRequest;
    std::chrono::steady_clock::time_point endRequest;
    std::chrono::steady_clock::time_point startParse;
    std::chrono::steady_clock::time_point endParse;

    int tries = 0;
    while (true && tries < 2) {
        auto now = std::chrono::steady_clock::now();
        // wait until 2 seconds have passed since the last request
        if (now - lastRequest < std::chrono::seconds(2)) {
            auto wait = std::chrono::seconds(2) - (now - lastRequest);
            std::this_thread::sleep_for(wait);
        }
        lastRequest = std::chrono::steady_clock::now();

        startRequest = std::chrono::high_resolution_clock::now();

        auto inventoryResponse = cpr::Get(cpr::Url{url}, cpr::Timeout{15000});

        endRequest = std::chrono::high_resolution_clock::now();

        if (inventoryResponse.status_code != 200) {
            if (inventoryResponse.status_code == 429) {
                consoleLog("Too many requests, waiting...", SEVERITY::WARNING);
                continue;
            }
            if (inventoryResponse.status_code == 403) {
                consoleLog("Inventory is private, aborting", SEVERITY::INFO);
                return;
            }
            consoleLog("Failed to fetch inventory (" + std::to_string(inventoryResponse.status_code) + "), aborting", SEVERITY::ERR);
            ++tries;
            continue;
        }

        if (inventoryResponse.error.code != cpr::ErrorCode::OK) {
            consoleLog("Failed to fetch inventory (" + inventoryResponse.error.message + "), aborting", SEVERITY::ERR);
            ++tries;
            continue;
        }

        if (tries >= 2) {
            consoleLog("Failed to fetch inventory after 2 tries, aborting", SEVERITY::ERR);
        }

        auto startParse = std::chrono::high_resolution_clock::now();
        inventoryData = nlohmann::json::parse(inventoryResponse.text);
        auto endParse = std::chrono::high_resolution_clock::now();
        break;
    }

    auto startItems = std::chrono::high_resolution_clock::now();
    if (inventoryData.total_inventory_count == 0 || inventoryData.assets == nullptr || inventoryData.descriptions == nullptr) {
        consoleLog("No items found", SEVERITY::INFO);
        return;
    }
    for (auto &asset : *inventoryData.assets) {
        for (auto &desc : *inventoryData.descriptions) {
            if (desc.classid == asset.classid) {
                if (desc.market_name == CURRENCY_SCRAP) {
                    scrapCount++;
                    break;
                }
                if (desc.market_name == CURRENCY_RECLAIMED) {
                    scrapCount += 3;
                    break;
                }
                if (desc.market_name == CURRENCY_REFINED) {
                    scrapCount += 9;
                    break;
                }
                if (desc.market_name == CURRENCY_KEY) {
                    keysCount++;
                    break;
                }

                Item item(desc);
                if (item.currency == TF2CURRENCY::NONE) break;
                if (item.getKeyPrice() < GLOBALS::scanner.config.getMinPriceInKeys()) break;
                if (item.tradable && !item.fail) items.push_back(item);
                break;
            }
        }
    }

    std::sort(items.begin(), items.end(), [](Item &a, Item &b) {
        return a.getKeyPrice() > b.getKeyPrice();
    });

    auto endItems = std::chrono::high_resolution_clock::now();

    auto durationRequest = std::chrono::duration_cast<std::chrono::milliseconds>(endRequest - startRequest);
    auto durationParse = std::chrono::duration_cast<std::chrono::milliseconds>(endParse - startParse);
    auto durationItems = std::chrono::duration_cast<std::chrono::milliseconds>(endItems - startItems);
    // consoleLog("Time taken to fetch inventory: " + std::to_string(durationRequest.count()) + "ms", SEVERITY::INFO);
    // consoleLog("Time taken to parse json: " + std::to_string(durationParse.count()) + "ms", SEVERITY::INFO);
    // consoleLog("Time taken to parse items: " + std::to_string(durationItems.count()) + "ms", SEVERITY::INFO);

    success = true;
}

void Inventory::ToConsole() {
    using namespace ImGui;
    if (TreeNode("Inventory")) {
        Text("Scrap: %d", scrapCount);
        Text("Keys: %d", keysCount);
        Text("Success: %s", success ? "Yes" : "No");
        std::string itemsName = "Items (" + std::to_string(items.size()) + ")";
        if (TreeNode(itemsName.c_str())) {
            InputText("Search", search, 32);
            for (Item &item : items) {
                if (item.name.find(search) == std::string::npos) continue;
                item.ToConsole();
            }
            TreePop();
        }
        TreePop();
    }
}
