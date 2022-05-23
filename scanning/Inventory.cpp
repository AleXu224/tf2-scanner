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

void Inventory::GetInventory() {
    consoleLog("Getting inventory for " + steamid, SEVERITY::INFO);

    std::string url = "https://steamcommunity.com/inventory/" + steamid + "/440/2?l=english&count=4000";

    JsonInventory::Inventory inventoryData;

    int tryCount = 0;
    std::chrono::steady_clock::time_point startRequest;
    std::chrono::steady_clock::time_point endRequest;
    std::chrono::steady_clock::time_point startParse;
    std::chrono::steady_clock::time_point endParse;

    while (true) {
        // TODO: add a timeout

        startRequest = std::chrono::high_resolution_clock::now();
        auto inventoryResponse = cpr::Get(cpr::Url{url});
        endRequest = std::chrono::high_resolution_clock::now();

        if (inventoryResponse.status_code != 200) {
            if (inventoryResponse.status_code == 429) {
                consoleLog("Too many requests, waiting...", SEVERITY::INFO);
                continue;
            }
            if (inventoryResponse.status_code == 403) {
                consoleLog("Inventory is private, aborting", SEVERITY::ERR);
                return;
            }
            consoleLog("Failed to fetch inventory (" + std::to_string(inventoryResponse.status_code) + "), aborting", SEVERITY::ERR);
            return;
        }

        auto startParse = std::chrono::high_resolution_clock::now();
        inventoryData = nlohmann::json::parse(inventoryResponse.text);
        auto endParse = std::chrono::high_resolution_clock::now();
        break;
    }

    auto startItems = std::chrono::high_resolution_clock::now();

    for (auto &asset : inventoryData.assets) {
        for (auto &desc : inventoryData.descriptions) {
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
    consoleLog("Time taken to fetch inventory: " + std::to_string(durationRequest.count()) + "ms", SEVERITY::INFO);
    consoleLog("Time taken to parse json: " + std::to_string(durationParse.count()) + "ms", SEVERITY::INFO);
    consoleLog("Time taken to parse items: " + std::to_string(durationItems.count()) + "ms", SEVERITY::INFO);

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
