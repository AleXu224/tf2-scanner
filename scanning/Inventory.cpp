#include "vector"
#include "Player.hpp"
#include "../globals.hpp"
#include "string"
#include "cpr/cpr.h"
#include "../json_schemas/InventoryData.hpp"
#include "vector"

#define consoleLog GLOBALS::console.addOutput

Inventory::Inventory(Player *player) : player(player){}

void Inventory::GetInventory() {
    consoleLog("Getting inventory for " + player->steamid, SEVERITY::INFO);

    std::string url = "https://steamcommunity.com/inventory/" + player->steamid + "/440/2?l=english&count=4000";

    JsonInventory::Inventory inventoryData;

    int tryCount = 0;
    while (true) {
        // TODO: add a timeout

        auto inventoryResponse = cpr::Get(cpr::Url{url});
        
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

        inventoryData = nlohmann::json::parse(inventoryResponse.text);
        break;
    }

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
                if (item.tradable && !item.fail) items.push_back(item);
                break;

            }
        }
    }

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
            for (Item &item : items) {
                item.ToConsole();
            }
            TreePop();
        }
        TreePop();
    }
}
