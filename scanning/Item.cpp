#include "Item.hpp"
#include "../globals.hpp"
#include "map"
#include "regex"

#define consoleLog GLOBALS::console.addOutput

const std::map<JsonInventory::LocalizedTagName, QUALITY> QUALITY_JSON = {
    {JsonInventory::LocalizedTagName::NORMAL, QUALITY::NORMAL},
    {JsonInventory::LocalizedTagName::GENUINE, QUALITY::GENIUNE},
    {JsonInventory::LocalizedTagName::VINTAGE, QUALITY::VINTAGE},
    {JsonInventory::LocalizedTagName::UNUSUAL, QUALITY::UNUSUAL},
    {JsonInventory::LocalizedTagName::UNIQUE, QUALITY::UNIQUE},
    {JsonInventory::LocalizedTagName::COMMUNITY, QUALITY::COMMUNITY},
    // No data for valve items sadly
    {JsonInventory::LocalizedTagName::SELF_MADE, QUALITY::SELF_MADE},
    {JsonInventory::LocalizedTagName::STRANGE, QUALITY::STRANGE},
    {JsonInventory::LocalizedTagName::HAUNTED, QUALITY::HAUNTED},
    {JsonInventory::LocalizedTagName::COLLECTOR_S, QUALITY::COLLECTORS},
    {JsonInventory::LocalizedTagName::DECORATED_WEAPON, QUALITY::DECORATED}
};

const std::map<JsonInventory::LocalizedTagName, WEAR> WEAR_JSON = {
    {JsonInventory::LocalizedTagName::BATTLE_SCARRED, WEAR::BATTLE_SCARRED},
    {JsonInventory::LocalizedTagName::WELL_WORN, WEAR::WELL_WORN},
    {JsonInventory::LocalizedTagName::FIELD_TESTED, WEAR::FIELD_TESTED},
    {JsonInventory::LocalizedTagName::MINIMAL_WEAR, WEAR::MINIMAL_WEAR},
    {JsonInventory::LocalizedTagName::FACTORY_NEW, WEAR::FACTORY_NEW},
};

Item::Item(JsonInventory::InventoryDescription &itemData)
{
    if (itemData.tradable == 0) tradable = false;

    int longestMatch = 0;
    for (auto &schemaItem : GLOBALS::scanner.config.itemSchema) {
        if (schemaItem.item_name == "") continue;
        if (itemData.market_name.find(schemaItem.item_name) != std::string::npos) {
            // Check if this is the longest match
            // this is needed for items like strangifiers
            if (schemaItem.item_name.size() < longestMatch) continue;
            longestMatch = schemaItem.item_name.size();
            
            nameSimple = schemaItem.item_name;
            properName = schemaItem.proper_name;
            defindex = schemaItem.defindex;
        }
        if (itemData.market_name.find(schemaItem.name) != std::string::npos) {
            // We also need to check this because for some reason
            // strangifiers have their data here instead of item_name
            if (schemaItem.name.size() < longestMatch) continue;
            longestMatch = schemaItem.name.size();
            
            nameSimple = schemaItem.name;
            properName = schemaItem.proper_name;
            defindex = schemaItem.defindex;
        }
    }
    if (longestMatch == 0) {
        consoleLog("Couldn't find item in schema: " + itemData.market_name, SEVERITY::ERR);
        fail = true;
        return;
    }

    name = workingName = itemData.market_name;

    // remove the name without prefixed or suffixes
    workingName.replace(workingName.find(nameSimple), nameSimple.length(), "");

    // Need to add the prefix of the url later on
    imageUrl = itemData.icon_url;

    // Killstreak
    if (workingName.find("Killstreak ") != std::string::npos) {
        killstreak = KILLSTREAK::BASIC;
        workingName.replace(workingName.find("Killstreak "), 11, "");

        if (workingName.find("Specialized ") != std::string::npos) {
            killstreak = KILLSTREAK::SPECIALIZED;
            workingName.replace(workingName.find("Specialized "), 12, "");
        } else if (workingName.find("Professional ") != std::string::npos) {
            killstreak = KILLSTREAK::PROFFESIONAL;
            workingName.replace(workingName.find("Professional "), 13, "");
        }
    }

    // Australium
    if (workingName.find("Australium ") != std::string::npos) {
        australium = true;
        workingName.replace(workingName.find("Australium "), 11, "");
    }

    // Quality, crate, wear check
    for (auto &tag : itemData.tags) {
        if (tag.category == JsonInventory::Category::QUALITY) {
            quality = QUALITY_JSON.at(tag.localized_tag_name);
            // Remove quality from name
            if (workingName.find(QUALITY_STRINGS.at(quality)) != std::string::npos) {
                workingName.replace(workingName.find(QUALITY_STRINGS.at(quality) + " "), QUALITY_STRINGS.at(quality).length() + 1, "");
            }
        } else if (tag.category == JsonInventory::Category::TYPE) {
            if (tag.localized_tag_name == JsonInventory::LocalizedTagName::CRATE)
                isCrate = true;
        } else if (tag.category == JsonInventory::Category::EXTERIOR) {
            skinWear = WEAR_JSON.at(tag.localized_tag_name);
            workingName.replace(workingName.find(" (" + WEAR_STRINGS.at(skinWear) + ")"), WEAR_STRINGS.at(skinWear).length() + 3, "");
            for (auto &skinData : GLOBALS::scanner.config.skinsData) {
                if (workingName.find(skinData.second) != std::string::npos) {
                    skinID = std::stoi(skinData.first);
                    workingName.replace(workingName.find(skinData.second + " "), skinData.second.length() + 1, "");
                    break;
                }
            }
            if (skinID == -1) {
                consoleLog("Couldn't find skin ID for: " + workingName, SEVERITY::ERR);
                fail = true;
                return;
            }
        }
    }

    // Unusual effect and craftable check
    if (itemData.descriptions) {
        for (auto &description : *itemData.descriptions) {
            if (quality == QUALITY::UNUSUAL && description.value.find("★ Unusual Effect: ") != std::string::npos) {
                effectName = description.value.substr(description.value.find("Unusual Effect: ") + 16);
                effectID = GLOBALS::scanner.config.itemEffects.at(effectName);
                if (effectID == -1) {
                    consoleLog("Couldn't find effect: " + effectName, SEVERITY::ERR);
                    fail = true;
                    return;
                }
                break;
            } else if (description.value.find("Usable in Crafting") != std::string::npos) {
                craftable = false;
            }
        }
    }

    // Check for secondary quality
    for (auto &qualityID : QUALITY_STRINGS) {
        if (workingName.find(qualityID.second + " ") != std::string::npos) {
            qualitySecondary = qualityID.first;
            workingName.replace(workingName.find(qualityID.second + " "), qualityID.second.length() + 1, "");
        }
    }

    // Get crate id if item is a crate
    if (isCrate) {
        // For some reason C++ regex doesn't support lookbehind
        // But it does support lookaheads, so we can just reverse the name
        std::regex crateRegex("[0-9]+(?=# )");

        std::string reversedString = workingName;
        std::reverse(reversedString.begin(), reversedString.end());
        std::smatch match;

        if (std::regex_search(reversedString, match, crateRegex)) {
            std::string crateIDstr = match.str();
            std::reverse(crateIDstr.begin(), crateIDstr.end());
            crateID = std::stoi(crateIDstr);
            workingName.replace(workingName.find(" Series #" + crateIDstr), crateIDstr.length() + 9, "");
        }
    }

    if (properName && quality == QUALITY::UNIQUE) {
        if (workingName.find("The ") != std::string::npos) {
            workingName.replace(workingName.find("The "), 4, "");
        }
    }

    if (workingName != "") {
        consoleLog("Unhandled name: (" + name + ") (" + workingName + ")", SEVERITY::ERR);
        fail = true;
        return;
    }

}

void Item::ToConsole() const {
    using namespace ImGui;
    if (TreeNode((void*)(intptr_t)this, name.c_str())) {
        Text("Tradable: %s", tradable ? "Yes" : "No");
        Text("Defindex: %d", defindex);
        Text("Name: %s", name.c_str());
        Text("Name Simple: %s", nameSimple.c_str());
        Text("Craftable: %s", craftable ? "Yes" : "No");
        Text("Australium: %s", australium ? "Yes" : "No");
        Text("Killstreak: %d", killstreak);
        Text("Quality: %d", quality);
        Text("Quality Secondary: %d", qualitySecondary);
        Text("Proper Name: %s", properName ? "Yes" : "No");
        Text("Is Crate: %s", isCrate ? "Yes" : "No");
        Text("Crate ID: %d", crateID);
        Text("Effect ID: %d", effectID);
        Text("Effect name: %s", effectName.c_str());
        Text("Skin ID: %d", skinID);
        Text("Skin Wear: %s", WEAR_STRINGS.at(skinWear).c_str());
        TreePop();
    }
}
