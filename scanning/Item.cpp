#include "Item.hpp"

#include "../globals.hpp"
#include "map"
#include "boost/regex.hpp"

#define consoleLog GLOBALS::console.addOutput

const std::map<std::string, QUALITY> QUALITY_JSON = {
    {"Normal", QUALITY::NORMAL},
    {"Genuine", QUALITY::GENIUNE},
    {"Vintage", QUALITY::VINTAGE},
    {"Unusual", QUALITY::UNUSUAL},
    {"Unique", QUALITY::UNIQUE},
    {"Community", QUALITY::COMMUNITY},
    {"Valve", QUALITY::VALVE},
    {"Self-Made", QUALITY::SELF_MADE},
    {"Strange", QUALITY::STRANGE},
    {"Haunted", QUALITY::HAUNTED},
    {"Colletor's", QUALITY::COLLECTORS},
    {"Decorated Weapon", QUALITY::DECORATED}};

const std::map<std::string, WEAR> WEAR_JSON = {
    {"Battle Scarred", WEAR::BATTLE_SCARRED},
    {"Well-Worn", WEAR::WELL_WORN},
    {"Field-Tested", WEAR::FIELD_TESTED},
    {"Minimal Wear", WEAR::MINIMAL_WEAR},
    {"Factory New", WEAR::FACTORY_NEW},
};

const std::map<std::string, TF2CURRENCY> CURRENCY_JSON = {
    {"hat", TF2CURRENCY::HATS},
    {"keys", TF2CURRENCY::KEYS},
    {"metal", TF2CURRENCY::METAL},
    {"usd", TF2CURRENCY::USD}};

Item::Item(JSON::SteamInventory::Description &itemData) {
    if (itemData.tradable == 0) tradable = false;

    name = workingName = itemData.market_name;

    // Quality, crate and tool, wear check
    for (auto &tag : itemData.tags) {
        if (tag.category == "Quality") {
            quality = QUALITY_JSON.at(tag.localized_tag_name);
            // Remove quality from name
            if (workingName.find(QUALITY_STRINGS.at(quality)) != std::string::npos) {
                workingName.replace(workingName.find(QUALITY_STRINGS.at(quality) + " "),
                                    QUALITY_STRINGS.at(quality).length() + 1, "");
            }
        } else if (tag.category == "Type") {
            if (tag.localized_tag_name == "Crate")
                isCrate = true;

        } else if (tag.category == "Exterior") {
            skinWear = WEAR_JSON.at(tag.localized_tag_name);
            workingName.replace(workingName.find(" (" + WEAR_STRINGS.at(skinWear) + ")"),
                                WEAR_STRINGS.at(skinWear).length() + 3, "");
            for (auto &skinData : GLOBALS::scanner.config.skinsData->skins) {
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

        std::unique_ptr<JSON::Schema::Item> foundItem;

        for (auto &schemaItem : GLOBALS::scanner.config.itemSchema->result.items) {
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
                foundItem = std::make_unique<JSON::Schema::Item>(schemaItem);
            }
        }

        if (longestName == 0)
            break;
        if (defindex == -1) {
            defindex = foundItem->defindex;
            nameSimple = foundItem->item_name;
            workingName.erase(earliestPosition, longestName);
            if (foundItem->proper_name) {
                auto findThe = workingName.find("The ");
                if (findThe != std::string::npos) {
                    workingName.erase(findThe, 4);
                }
            }
        } else if (toolDefindex == -1) {
            toolDefindex = foundItem->defindex;
            toolName = foundItem->item_name;
            workingName.erase(earliestPosition - 1, longestName + 1);
        } else if (recipeDefindex == -1) {
            recipeDefindex = foundItem->defindex;
            recipeName = foundItem->item_name;
            workingName.erase(earliestPosition - 1, longestName + 1);
        } else {
            consoleLog("Too many arguments found: (" + workingName + ") (" + foundItem->item_name + ")", SEVERITY::ERR);
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
                if (GLOBALS::scanner.config.itemEffects->effects.find(effectName) == GLOBALS::scanner.config.itemEffects->effects.end()) {
                    consoleLog("Couldn't find effect: " + effectName, SEVERITY::ERR);
                    fail = true;
                    return;
                }
                effectID = GLOBALS::scanner.config.itemEffects->effects.at(effectName);
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
    // std::regex crateRegex("[0-9]+(?=# )");

    // std::string reversedString = workingName;
    // std::reverse(reversedString.begin(), reversedString.end());
    // std::smatch match;

    boost::regex crateRegex("(?<=#)[0-9]+");
    boost::smatch crateMatch;

    // if (std::regex_search(reversedString, match, crateRegex)) {
    if (boost::regex_search(workingName, crateMatch, crateRegex)) {
        std::string crateIDstr = crateMatch.str();
        // std::reverse(crateIDstr.begin(), crateIDstr.end());
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


    if ((skinID != -1 || name.find("Killstreak ") != std::string::npos) && tradable) {
        for (auto &marketPrice : GLOBALS::scanner.config.marketPrices->prices) {
            if (marketPrice.name == name) {
                price = static_cast<float>(marketPrice.sell_price) / 100; // Convert from cents to dollars
                currency = TF2CURRENCY::USD;
                if (name.find("Unusual ") != std::string::npos && effectID != -1) {
                    name.replace(name.find("Unusual"), 7, effectName);
                }
                return;
            }
        }
        consoleLog("Couldn't find price for: " + name, SEVERITY::WARNING);
        return;
    }

    if (name.find("Unusual ") != std::string::npos && effectID != -1) {
        name.replace(name.find("Unusual"), 7, effectName);
    }

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

    auto &prices = GLOBALS::scanner.config.itemPrices->response.items;
    if (prices.find(priceName) != prices.end()) {
        auto postName = prices.at(priceName).prices;
        std::optional<JSON::BptfPrices::Tradable> postQuality;

        if (postName.find(std::to_string((int)qualitySecondary)) != postName.end()) {
            postQuality = postName.at(std::to_string((int)qualitySecondary)).tradable;
        } else if (postName.find(std::to_string((int)quality)) != postName.end()) {
            postQuality = postName.at(std::to_string((int)quality)).tradable;
        } else {
            return;
        }

        std::optional<std::variant<std::map<std::string, JSON::BptfPrices::Element>, std::vector<JSON::BptfPrices::Element>>> postCraftable;
        if (craftable)
            postCraftable = postQuality->craftable;
        else
            postCraftable = postQuality->non_craftable;

        if (!postCraftable.has_value()) return;
        // JsonPrices::CraftableElement postId;
        std::optional<JSON::BptfPrices::Element> postId;
        if (std::holds_alternative<std::vector<JSON::BptfPrices::Element>>(postCraftable.value())) {
            // vector
            postId = std::get<std::vector<JSON::BptfPrices::Element>>(postCraftable.value()).at(0);
        } else {
            // map
            auto map = std::get<std::map<std::string, JSON::BptfPrices::Element>>(postCraftable.value());
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
        if (!postId->currency.has_value() || !postId->value.has_value())
            return;
        currency = CURRENCY_JSON.at(postId->currency.value());
        price = postId->value.value();
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
#define PRICES GLOBALS::scanner.config.itemPrices->response.items
    auto keyPriceCraftable = PRICES.at(CURRENCY_KEY).prices.at("6").tradable.craftable;
    auto keyPriceVector = std::get<std::vector<JSON::BptfPrices::Element>>(keyPriceCraftable.value());
    auto keyPrice = keyPriceVector.at(0).value.value();

    if (currency == TF2CURRENCY::NONE)
        return 0.0f;
    else if (currency == TF2CURRENCY::KEYS)
        return price;
    else if (currency == TF2CURRENCY::METAL)
        return price / keyPrice;
    else if (currency == TF2CURRENCY::HATS)
        return (4.0f / 3.0f) / keyPrice;
    else if (currency == TF2CURRENCY::USD)
        return price / 2.2f; // placeholder key price, should work well enough
    else
        return 0.0f; // literally can't happen but the compiler won't stop bugging me about it
}
