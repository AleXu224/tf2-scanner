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
    {JsonInventory::LocalizedTagName::DECORATED_WEAPON, QUALITY::DECORATED}};

const std::map<JsonInventory::LocalizedTagName, WEAR> WEAR_JSON = {
    {JsonInventory::LocalizedTagName::BATTLE_SCARRED, WEAR::BATTLE_SCARRED},
    {JsonInventory::LocalizedTagName::WELL_WORN, WEAR::WELL_WORN},
    {JsonInventory::LocalizedTagName::FIELD_TESTED, WEAR::FIELD_TESTED},
    {JsonInventory::LocalizedTagName::MINIMAL_WEAR, WEAR::MINIMAL_WEAR},
    {JsonInventory::LocalizedTagName::FACTORY_NEW, WEAR::FACTORY_NEW},
};

const std::map<JsonPrices::Currency, TF2CURRENCY> CURRENCY_JSON = {
    {JsonPrices::Currency::HAT, TF2CURRENCY::HATS},
    {JsonPrices::Currency::KEYS, TF2CURRENCY::KEYS},
    {JsonPrices::Currency::METAL, TF2CURRENCY::METAL},
    {JsonPrices::Currency::USD, TF2CURRENCY::USD}};

Item::Item(JsonInventory::InventoryDescription &itemData) {
    if (itemData.tradable == 0) tradable = false;

    name = workingName = itemData.market_name;

    // Quality, crate and tool, wear check
    for (auto &tag : itemData.tags) {
        if (tag.category == JsonInventory::Category::QUALITY) {
            quality = QUALITY_JSON.at(tag.localized_tag_name);
            // Remove quality from name
            if (workingName.find(QUALITY_STRINGS.at(quality)) != std::string::npos) {
                workingName.replace(workingName.find(QUALITY_STRINGS.at(quality) + " "),
                                    QUALITY_STRINGS.at(quality).length() + 1, "");
            }
        } else if (tag.category == JsonInventory::Category::TYPE) {
            if (tag.localized_tag_name == JsonInventory::LocalizedTagName::CRATE)
                isCrate = true;

        } else if (tag.category == JsonInventory::Category::EXTERIOR) {
            skinWear = WEAR_JSON.at(tag.localized_tag_name);
            workingName.replace(workingName.find(" (" + WEAR_STRINGS.at(skinWear) + ")"),
                                WEAR_STRINGS.at(skinWear).length() + 3, "");
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

    while (true) {
        int longestName = 0;
        int earliestPosition = workingName.size();

        JsonSchema::Item foundItem;

        for (auto &schemaItem : GLOBALS::scanner.config.itemSchema) {
            if (schemaItem.item_name.empty())
                continue;
            auto find1 = workingName.find(schemaItem.item_name);
            if (find1 != std::string::npos) {
                if (find1 > earliestPosition)
                    continue;
                if (find1 == earliestPosition && schemaItem.item_name.size() <= longestName)
                    continue;

                earliestPosition = find1;
                longestName = schemaItem.item_name.size();
                foundItem = schemaItem;
            }
        }

        if (longestName == 0)
            break;
        if (defindex == -1) {
            defindex = foundItem.defindex;
            nameSimple = foundItem.item_name;
            workingName.erase(earliestPosition, longestName);
            if (foundItem.proper_name) {
                auto findThe = workingName.find("The ");
                if (findThe != std::string::npos) {
                    workingName.erase(findThe, 4);
                }
            }
        } else if (toolDefindex == -1) {
            toolDefindex = foundItem.defindex;
            toolName = foundItem.item_name;
            workingName.erase(earliestPosition - 1, longestName + 1);
        } else if (recipeDefindex == -1) {
            recipeDefindex = foundItem.defindex;
            recipeName = foundItem.item_name;
            workingName.erase(earliestPosition - 1, longestName + 1);
        } else {
            consoleLog("Too many arguments found: (" + workingName + ") (" + foundItem.item_name + ")", SEVERITY::ERR);
            break;
        }
    }

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

    if (quality == QUALITY::STRANGE && skinID != -1) {
        quality = QUALITY::DECORATED;
        qualitySecondary = QUALITY::STRANGE;
    }

    // Get crate id if item is a crate
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

    if (properName && quality == QUALITY::UNIQUE) {
        if (workingName.find("The ") != std::string::npos) {
            workingName.replace(workingName.find("The "), 4, "");
        }
    }

    if (workingName.find("Festivized ") != std::string::npos) {
        isFestivized = true;
        workingName.erase(workingName.find("Festivized "), 11);
    }

    if (!workingName.empty()) {
        consoleLog("Unhandled name: (" + name + ") (" + workingName + ")", SEVERITY::ERR);
        fail = true;
        return;
    }

    if (name.find("Unusual ") != std::string::npos && effectID != -1) {
        name.replace(name.find("Unusual"), 7, effectName);
    }

    // skins don't have prices so don't need to waste resources on that
    if (quality == QUALITY::DECORATED || qualitySecondary == QUALITY::DECORATED) return;

    if (!tradable) return;

    std::string priceName;
    if (recipeDefindex != -1) {
        priceName = recipeName;
    } else if (toolDefindex != -1) {
        priceName = toolName;
    } else {
        priceName = nameSimple;
    }

    if (australium)
        priceName = "Australium " + priceName;

    auto &prices = GLOBALS::scanner.config.itemPrices.response.items;
    if (prices.find(priceName) != prices.end()) {
        auto postName = prices.at(priceName).prices;
        JsonPrices::Tradable postQuality;

        if (postName.find(std::to_string((int)qualitySecondary)) != postName.end()) {
            postQuality = postName.at(std::to_string((int)qualitySecondary)).tradable;
        } else if (postName.find(std::to_string((int)quality)) != postName.end()) {
            postQuality = postName.at(std::to_string((int)quality)).tradable;
        } else {
            return;
        }

        std::shared_ptr<JsonPrices::CraftableUnion> postCraftable;
        if (craftable)
            postCraftable = postQuality.craftable;
        else
            postCraftable = postQuality.non_craftable;

        if (postCraftable == nullptr) return;
        JsonPrices::CraftableElement postId;
        auto element = std::get_if<std::vector<JsonPrices::CraftableElement>>(&(*postCraftable));
        if (element != nullptr) {
            // vector
            postId = element->at(0);
        } else {
            // map
            auto map = std::get<std::map<std::string, JsonPrices::CraftableElement>>(*postCraftable);
            if ((recipeDefindex != -1 || toolDefindex != -1)) {
                if (map.find(std::to_string(defindex)) == map.end()) return;
                postId = map.at(std::to_string(defindex));
            } else if (effectID != -1) {
                if (map.find(std::to_string(effectID)) == map.end()) return;
                postId = map.at(std::to_string(effectID));
            } else if (crateID != -1) {
                if (map.find(std::to_string(crateID)) == map.end()) return;
                postId = map.at(std::to_string(crateID));
            } else if (isCrate) {
                // If this is a crate but we don't have a crateID
                // then there will be only one element
                postId = map.begin()->second;
            } else if (map.find("0") != map.end()) {
                postId = map.at("0");
            } else {
                return;
            }
        }
        if (postId.currency == nullptr || postId.value == nullptr)
            return;
        currency = CURRENCY_JSON.at(*postId.currency);
        price = *postId.value;
    }
}

void Item::ToConsole() const {
    using namespace ImGui;
    if (TreeNode((void *)(intptr_t)this, name.c_str())) {
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
        Text("Tool Defindex: %d", toolDefindex);
        Text("Tool Name: %s", toolName.c_str());
        Text("Recipe Defindex: %d", recipeDefindex);
        Text("Recipe Name: %s", recipeName.c_str());
        Text("Price: %f", price);
        Text("Currency: %s", TF2CURRENCY_STRINGS.at(currency).c_str());
        TreePop();
    }
}

float Item::getKeyPrice() {
#define PRICES GLOBALS::scanner.config.itemPrices.response.items
    auto keyPriceCraftable = PRICES.at(CURRENCY_KEY).prices.at("6").tradable.craftable;
    auto keyPriceVector = std::get<std::vector<JsonPrices::CraftableElement>>(*keyPriceCraftable);
    auto keyPrice = *keyPriceVector.at(0).value;

    if (currency == TF2CURRENCY::NONE)
        return 0.0f;
    else if (currency == TF2CURRENCY::KEYS)
        return price;
    else if (currency == TF2CURRENCY::METAL)
        return price / keyPrice;
    else
        return (4.0f / 3.0f) / keyPrice;
}
