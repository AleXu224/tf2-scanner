//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Inventories data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

#include <unordered_map>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann
{
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>>
    {
        static void to_json(json &j, const std::shared_ptr<T> &opt)
        {
            if (!opt)
                j = nullptr;
            else
                j = *opt;
        }

        static std::shared_ptr<T> from_json(const json &j)
        {
            if (j.is_null())
                return std::unique_ptr<T>();
            else
                return std::unique_ptr<T>(new T(j.get<T>()));
        }
    };
}
#endif

namespace JsonInventory
{
    using nlohmann::json;

    inline json get_untyped(const json &j, const char *property)
    {
        if (j.find(property) != j.end())
        {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json &j, std::string property)
    {
        return get_untyped(j, property.data());
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json &j, const char *property)
    {
        if (j.find(property) != j.end())
        {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json &j, std::string property)
    {
        return get_optional<T>(j, property.data());
    }

    struct Asset
    {
        int64_t appid;
        std::string contextid;
        std::string assetid;
        std::string classid;
        std::string instanceid;
        std::string amount;
    };

    enum class Name : int
    {
        INSPECT_IN_GAME,
        ITEM_WIKI_PAGE
    };

    struct Action
    {
        std::string link;
        Name name;
    };

    enum class BackgroundColor : int
    {
        THE_3_C352_E
    };

    enum class Color : int
    {
        B0_C3_D9,
        CF6_A32,
        D32_CE6,
        D83636,
        E1_E10_F,
        EB4_B4_B,
        FFD700,
        THE_00_A000,
        THE_4_B69_FF,
        THE_5_E98_D9,
        THE_6_F6_A63,
        THE_756_B5_E,
        THE_7_D6_D00,
        THE_7_EA9_D1,
        THE_8847_FF,
        THE_8_B8989
    };

    struct DescriptionDescription
    {
        std::string value;
        std::shared_ptr<Color> color;
    };

    enum class Category : int
    {
        CLASS,
        COLLECTION,
        EXTERIOR,
        GRADE,
        QUALITY,
        RARITY,
        TYPE
    };

    enum class InternalName : int
    {
        ACTION,
        BLUE_MOON_COSMETICS_COLLECTION,
        BUILDING,
        CAMPAIGN3_COSMETICS_CASE1_COLLECTION,
        CAMPAIGN3_COSMETICS_CASE2_COLLECTION,
        CAMPAIGN3_MASTER_COLLECTION,
        CAMPAIGN3_PAINTKITS_CASE1_COLLECTION,
        CAMPAIGN3_PAINTKITS_CONTRACT1_COLLECTION,
        CAMPAIGN3_PAINTKITS_CONTRACT2_COLLECTION,
        COLLECTORS,
        COMMUNITY,
        CONCEALEDKILLER_COLLECTION,
        CRAFTSMANN_COLLECTION,
        CRAFT_ITEM,
        DEMOMAN,
        EMPTY,
        ENGINEER,
        GUN_METTLE_COSMETICS_COLLECTION,
        HALLOWEEN2015_COLLECTION_NAME,
        HALLOWEEN2016_COLLECTION_NAME,
        HALLOWEEN2018_COLLECTION_NAME,
        HALLOWEEN2018_PAINTKITS_COLLECTION,
        HALLOWEEN2019_COLLECTION_NAME,
        HALLOWEEN2020_COLLECTION_NAME,
        HALLOWEEN2020_PAINTKITS_COLLECTION,
        HALLOWEEN2021_COLLECTION_NAME,
        HALLOWEEN2021_PAINTKITS_COLLECTION,
        HALLOWEEN_MASTER_COLLECTION,
        HAUNTED,
        HEAVY,
        INTERNAL_NAME_TF_GIFT,
        INVASION_COLLECTION_01,
        INVASION_COLLECTION_02,
        INVASION_MASTER_COLLECTION,
        MAYFLOWER_COSMETICS_COLLECTION,
        MEDIC,
        MELEE,
        MISC,
        NORMAL,
        OPERATION_GUN_METTLE_MASTER_COLLECTION,
        OPERATION_TOUGH_BREAK_MASTER_COLLECTION,
        PAINTKITWEAPON,
        PDA,
        PDA2,
        POWERHOUSE_COLLECTION,
        PRIMARY,
        PYRO,
        RAINY_DAY_COSMETICS_COLLECTION,
        RARITY1,
        RARITY4,
        RARITY_ANCIENT,
        RARITY_COMMON,
        RARITY_LEGENDARY,
        RARITY_MYTHICAL,
        RARITY_RARE,
        RARITY_UNCOMMON,
        SCOUT,
        SECONDARY,
        SELFMADE,
        SNIPER,
        SOLDIER,
        SPY,
        STRANGE,
        SUMMER2019_COSMETICS_COLLECTION,
        SUMMER2020_COSMETICS_COLLECTION,
        SUMMER2021_COSMETICS_COLLECTION,
        SUPPLY_CRATE,
        TAUNT,
        TEUFORT_COLLECTION,
        TFUI_INV_TOOLTIP_BATTLE_SCARED,
        TFUI_INV_TOOLTIP_FACTORY_NEW,
        TFUI_INV_TOOLTIP_FIELD_TESTED,
        TFUI_INV_TOOLTIP_MINIMAL_WEAR,
        TFUI_INV_TOOLTIP_WELL_WORN,
        TF_FESTIVIZER_TOOL,
        TF_GIFT,
        TF_ITEM_DYNAMIC_RECIPE_TOOL,
        TF_KILL_STREAKIFIER_TOOL_A,
        TF_KILL_STREAKIFIER_TOOL_C,
        TF_LOCKED_CRATE,
        TF_PACKAGE_TYPE,
        TF_PAINT_KIT_TOOL,
        TF_PARTY_FAVOR,
        TF_SERVER_ENCHANTMENT_TYPE,
        TF_SPELLBOOK_PAGE_TYPE,
        TF_STRANGE_PART,
        TF_STRANGIFIER_TOOL,
        TF_T,
        TF_UNUSUALIFIER,
        TF_USABLE_ITEM,
        TOUGH_BREAK_CASE_COLLECTION_01,
        TOUGH_BREAK_COSMETICS_COLLECTION,
        TOUGH_BREAK_DROP_COLLECTION_01,
        TOUGH_BREAK_DROP_COLLECTION_02,
        UNIQUE,
        UNIQUE_FREE_SKINS_COLLECTION,
        VINTAGE,
        WINTER2016_COSMETICS_COLLECTION,
        WINTER2017_COSMETICS_COLLECTION,
        WINTER2018_COSMETICS_COLLECTION,
        WINTER2019_COSMETICS_COLLECTION,
        WINTER2019_PAINTKITS_COLLECTION,
        WINTER2020_COSMETICS_COLLECTION,
        WINTER2020_PAINTKITS_COLLECTION,
        WINTER2021_COSMETICS_COLLECTION
    };

    enum class LocalizedTagName : int
    {
        ABOMINABLE_COSMETIC_COLLECTION,
        ACTION,
        ASSASSIN,
        BATTLE_SCARRED,
        BLUE_MOON_COSMETICS_COLLECTION,
        BUILDING,
        CIVILIAN,
        COLLECTOR_S,
        COMMANDO,
        COMMUNITY,
        CONCEALED_KILLER_COLLECTION,
        CONFIDENTIAL_COLLECTION_CASE,
        CONTRACT_CAMPAIGNER_COLLECTION,
        COSMETIC,
        CRAFTSMANN_COLLECTION,
        CRAFT_ITEM,
        CRATE,
        CREEPY_CRAWLY_COLLECTION,
        CRIMSON_CACHE_COLLECTION,
        DECORATED_WAR_HERO_COLLECTION,
        DECORATED_WEAPON,
        DEMOMAN,
        ELITE,
        EMPTY,
        ENGINEER,
        FACTORY_NEW,
        FESTIVIZER,
        FIELD_TESTED,
        FREELANCE,
        GARGOYLE_COLLECTION,
        GENTLEMANNE_S_COLLECTION,
        GENUINE,
        GIFT,
        GUN_METTLE_COSMETICS_COLLECTION,
        HARVEST_COLLECTION,
        HAUNTED,
        HEAVY,
        INVASION_COLLECTIONS,
        JUNGLE_INFERNO_COLLECTIONS,
        JUNGLE_JACKPOT_COLLECTION,
        KILLSTREAK_KIT,
        MANN_CO_EVENTS_COLLECTION,
        MAYFLOWER_COSMETICS_COLLECTION,
        MEDIC,
        MELEE_WEAPON,
        MERCENARY,
        MINIMAL_WEAR,
        NORMAL,
        PACKAGE,
        PARTY_FAVOR,
        POWERHOUSE_COLLECTION,
        PRIMARY_PDA,
        PRIMARY_WEAPON,
        PROFESSIONAL_KILLSTREAK_KIT,
        PYRO,
        PYROLAND_COLLECTION,
        QUARANTINED_COLLECTION_CASE,
        RAINY_DAY_COSMETICS_COLLECTION,
        RECIPE,
        SCOUT,
        SCREAM_FORTRESS_XIII_COLLECTION,
        SCREAM_FORTRESS_XII_COLLECTION,
        SCREAM_FORTRESS_X_COLLECTION,
        SECONDARY_PDA,
        SECONDARY_WEAPON,
        SELF_MADE,
        SERVER_ENCHANTMENT,
        SNIPER,
        SOLDIER,
        SPELLBOOK_PAGE,
        SPOOKY_SPOILS_COLLECTION,
        SPY,
        STRANGE,
        STRANGE_PART,
        STRANGIFIER,
        SUMMER_2019_COSMETICS_COLLECTION,
        SUMMER_2020_COSMETICS_COLLECTION,
        SUMMER_2021_COSMETICS_COLLECTION,
        SUPPLY_CRATE,
        TAUNT_1,
        TEUFORT_COLLECTION,
        THE_GUN_METTLE_COLLECTIONS,
        THE_SCREAM_FORTRESS_COLLECTIONS,
        THE_TOUGH_BREAK_COLLECTIONS,
        TOOL,
        TOUGH_BREAK_COSMETICS_COLLECTION,
        UNIQUE,
        UNLEASH_THE_BEAST_COSMETIC_COLLECTION,
        UNUSUAL,
        UNUSUALIFIER,
        USABLE_ITEM,
        VINTAGE,
        VIOLET_VERMIN_COLLECTION,
        WAR_PAINT,
        WELL_WORN,
        WICKED_WINDFALL_COLLECTION,
        WINTER_2016_COSMETICS_COLLECTION,
        WINTER_2017_COSMETICS_COLLECTION,
        WINTER_2018_COSMETICS_COLLECTION,
        WINTER_2019_COLLECTION,
        WINTER_2019_COSMETICS_COLLECTION,
        WINTER_2020_COLLECTION,
        WINTER_2020_COSMETICS_COLLECTION,
        WINTER_2021_COSMETICS_COLLECTION
    };

    struct Tag
    {
        Category category;
        InternalName internal_name;
        Category localized_category_name;
        LocalizedTagName localized_tag_name;
        std::shared_ptr<std::string> color;
    };

    struct InventoryDescription
    {
        int64_t appid;
        std::string classid;
        std::string instanceid;
        int64_t currency;
        BackgroundColor background_color;
        std::string icon_url;
        std::string icon_url_large;
        int64_t tradable;
        std::vector<Action> actions;
        std::string name;
        std::string name_color;
        std::string type;
        std::string market_name;
        std::string market_hash_name;
        int64_t commodity;
        int64_t market_tradable_restriction;
        int64_t market_marketable_restriction;
        int64_t marketable;
        std::vector<Tag> tags;
        std::shared_ptr<std::vector<DescriptionDescription>> descriptions;
        std::shared_ptr<std::vector<Action>> market_actions;
        std::shared_ptr<std::vector<std::string>> fraudwarnings;
    };

    struct Inventory
    {
        std::vector<Asset> assets;
        std::vector<InventoryDescription> descriptions;
        int64_t total_inventory_count;
        int64_t success;
        int64_t rwgrsn;
    };

    using Inventories = std::vector<Inventory>;
}

namespace JsonInventory
{
    using Inventories = std::vector<Inventory>;
}

namespace nlohmann
{
    void from_json(const json &j, JsonInventory::Asset &x);
    void to_json(json &j, const JsonInventory::Asset &x);

    void from_json(const json &j, JsonInventory::Action &x);
    void to_json(json &j, const JsonInventory::Action &x);

    void from_json(const json &j, JsonInventory::DescriptionDescription &x);
    void to_json(json &j, const JsonInventory::DescriptionDescription &x);

    void from_json(const json &j, JsonInventory::Tag &x);
    void to_json(json &j, const JsonInventory::Tag &x);

    void from_json(const json &j, JsonInventory::InventoryDescription &x);
    void to_json(json &j, const JsonInventory::InventoryDescription &x);

    void from_json(const json &j, JsonInventory::Inventory &x);
    void to_json(json &j, const JsonInventory::Inventory &x);

    void from_json(const json &j, JsonInventory::Name &x);
    void to_json(json &j, const JsonInventory::Name &x);

    void from_json(const json &j, JsonInventory::BackgroundColor &x);
    void to_json(json &j, const JsonInventory::BackgroundColor &x);

    void from_json(const json &j, JsonInventory::Color &x);
    void to_json(json &j, const JsonInventory::Color &x);

    void from_json(const json &j, JsonInventory::Category &x);
    void to_json(json &j, const JsonInventory::Category &x);

    void from_json(const json &j, JsonInventory::InternalName &x);
    void to_json(json &j, const JsonInventory::InternalName &x);

    void from_json(const json &j, JsonInventory::LocalizedTagName &x);
    void to_json(json &j, const JsonInventory::LocalizedTagName &x);

    inline void from_json(const json &j, JsonInventory::Asset &x)
    {
        x.appid = j.at("appid").get<int64_t>();
        x.contextid = j.at("contextid").get<std::string>();
        x.assetid = j.at("assetid").get<std::string>();
        x.classid = j.at("classid").get<std::string>();
        x.instanceid = j.at("instanceid").get<std::string>();
        x.amount = j.at("amount").get<std::string>();
    }

    inline void to_json(json &j, const JsonInventory::Asset &x)
    {
        j = json::object();
        j["appid"] = x.appid;
        j["contextid"] = x.contextid;
        j["assetid"] = x.assetid;
        j["classid"] = x.classid;
        j["instanceid"] = x.instanceid;
        j["amount"] = x.amount;
    }

    inline void from_json(const json &j, JsonInventory::Action &x)
    {
        x.link = j.at("link").get<std::string>();
        x.name = j.at("name").get<JsonInventory::Name>();
    }

    inline void to_json(json &j, const JsonInventory::Action &x)
    {
        j = json::object();
        j["link"] = x.link;
        j["name"] = x.name;
    }

    inline void from_json(const json &j, JsonInventory::DescriptionDescription &x)
    {
        x.value = j.at("value").get<std::string>();
        x.color = JsonInventory::get_optional<JsonInventory::Color>(j, "color");
    }

    inline void to_json(json &j, const JsonInventory::DescriptionDescription &x)
    {
        j = json::object();
        j["value"] = x.value;
        j["color"] = x.color;
    }

    inline void from_json(const json &j, JsonInventory::Tag &x)
    {
        x.category = j.at("category").get<JsonInventory::Category>();
        x.internal_name = j.at("internal_name").get<JsonInventory::InternalName>();
        x.localized_category_name = j.at("localized_category_name").get<JsonInventory::Category>();
        x.localized_tag_name = j.at("localized_tag_name").get<JsonInventory::LocalizedTagName>();
        x.color = JsonInventory::get_optional<std::string>(j, "color");
    }

    inline void to_json(json &j, const JsonInventory::Tag &x)
    {
        j = json::object();
        j["category"] = x.category;
        j["internal_name"] = x.internal_name;
        j["localized_category_name"] = x.localized_category_name;
        j["localized_tag_name"] = x.localized_tag_name;
        j["color"] = x.color;
    }

    inline void from_json(const json &j, JsonInventory::InventoryDescription &x)
    {
        x.appid = j.at("appid").get<int64_t>();
        x.classid = j.at("classid").get<std::string>();
        x.instanceid = j.at("instanceid").get<std::string>();
        x.currency = j.at("currency").get<int64_t>();
        x.background_color = j.at("background_color").get<JsonInventory::BackgroundColor>();
        x.icon_url = j.at("icon_url").get<std::string>();
        x.icon_url_large = j.at("icon_url_large").get<std::string>();
        x.tradable = j.at("tradable").get<int64_t>();
        x.actions = j.at("actions").get<std::vector<JsonInventory::Action>>();
        x.name = j.at("name").get<std::string>();
        x.name_color = j.at("name_color").get<std::string>();
        x.type = j.at("type").get<std::string>();
        x.market_name = j.at("market_name").get<std::string>();
        x.market_hash_name = j.at("market_hash_name").get<std::string>();
        x.commodity = j.at("commodity").get<int64_t>();
        x.market_tradable_restriction = j.at("market_tradable_restriction").get<int64_t>();
        x.market_marketable_restriction = j.at("market_marketable_restriction").get<int64_t>();
        x.marketable = j.at("marketable").get<int64_t>();
        x.tags = j.at("tags").get<std::vector<JsonInventory::Tag>>();
        x.descriptions = JsonInventory::get_optional<std::vector<JsonInventory::DescriptionDescription>>(j, "descriptions");
        x.market_actions = JsonInventory::get_optional<std::vector<JsonInventory::Action>>(j, "market_actions");
        x.fraudwarnings = JsonInventory::get_optional<std::vector<std::string>>(j, "fraudwarnings");
    }

    inline void to_json(json &j, const JsonInventory::InventoryDescription &x)
    {
        j = json::object();
        j["appid"] = x.appid;
        j["classid"] = x.classid;
        j["instanceid"] = x.instanceid;
        j["currency"] = x.currency;
        j["background_color"] = x.background_color;
        j["icon_url"] = x.icon_url;
        j["icon_url_large"] = x.icon_url_large;
        j["tradable"] = x.tradable;
        j["actions"] = x.actions;
        j["name"] = x.name;
        j["name_color"] = x.name_color;
        j["type"] = x.type;
        j["market_name"] = x.market_name;
        j["market_hash_name"] = x.market_hash_name;
        j["commodity"] = x.commodity;
        j["market_tradable_restriction"] = x.market_tradable_restriction;
        j["market_marketable_restriction"] = x.market_marketable_restriction;
        j["marketable"] = x.marketable;
        j["tags"] = x.tags;
        j["descriptions"] = x.descriptions;
        j["market_actions"] = x.market_actions;
        j["fraudwarnings"] = x.fraudwarnings;
    }

    inline void from_json(const json &j, JsonInventory::Inventory &x)
    {
        x.assets = j.at("assets").get<std::vector<JsonInventory::Asset>>();
        x.descriptions = j.at("descriptions").get<std::vector<JsonInventory::InventoryDescription>>();
        x.total_inventory_count = j.at("total_inventory_count").get<int64_t>();
        x.success = j.at("success").get<int64_t>();
        x.rwgrsn = j.at("rwgrsn").get<int64_t>();
    }

    inline void to_json(json &j, const JsonInventory::Inventory &x)
    {
        j = json::object();
        j["assets"] = x.assets;
        j["descriptions"] = x.descriptions;
        j["total_inventory_count"] = x.total_inventory_count;
        j["success"] = x.success;
        j["rwgrsn"] = x.rwgrsn;
    }

    inline void from_json(const json &j, JsonInventory::Name &x)
    {
        if (j == "Inspect in Game...")
            x = JsonInventory::Name::INSPECT_IN_GAME;
        else if (j == "Item Wiki Page...")
            x = JsonInventory::Name::ITEM_WIKI_PAGE;
        else
            throw "Input JSON does not conform to schema";
    }

    inline void to_json(json &j, const JsonInventory::Name &x)
    {
        switch (x)
        {
        case JsonInventory::Name::INSPECT_IN_GAME:
            j = "Inspect in Game...";
            break;
        case JsonInventory::Name::ITEM_WIKI_PAGE:
            j = "Item Wiki Page...";
            break;
        default:
            throw "This should not happen";
        }
    }

    inline void from_json(const json &j, JsonInventory::BackgroundColor &x)
    {
        if (j == "3C352E")
            x = JsonInventory::BackgroundColor::THE_3_C352_E;
        else
            throw "Input JSON does not conform to schema";
    }

    inline void to_json(json &j, const JsonInventory::BackgroundColor &x)
    {
        switch (x)
        {
        case JsonInventory::BackgroundColor::THE_3_C352_E:
            j = "3C352E";
            break;
        default:
            throw "This should not happen";
        }
    }

    inline void from_json(const json &j, JsonInventory::Color &x)
    {
        static std::unordered_map<std::string, JsonInventory::Color> enumValues{
            {"b0c3d9", JsonInventory::Color::B0_C3_D9},
            {"CF6A32", JsonInventory::Color::CF6_A32},
            {"d32ce6", JsonInventory::Color::D32_CE6},
            {"d83636", JsonInventory::Color::D83636},
            {"e1e10f", JsonInventory::Color::E1_E10_F},
            {"eb4b4b", JsonInventory::Color::EB4_B4_B},
            {"ffd700", JsonInventory::Color::FFD700},
            {"00a000", JsonInventory::Color::THE_00_A000},
            {"4b69ff", JsonInventory::Color::THE_4_B69_FF},
            {"5e98d9", JsonInventory::Color::THE_5_E98_D9},
            {"6F6A63", JsonInventory::Color::THE_6_F6_A63},
            {"756b5e", JsonInventory::Color::THE_756_B5_E},
            {"7D6D00", JsonInventory::Color::THE_7_D6_D00},
            {"7ea9d1", JsonInventory::Color::THE_7_EA9_D1},
            {"8847ff", JsonInventory::Color::THE_8847_FF},
            {"8b8989", JsonInventory::Color::THE_8_B8989},
        };
        auto iter = enumValues.find(j.get<std::string>());
        if (iter != enumValues.end())
        {
            x = iter->second;
        }
    }

    inline void to_json(json &j, const JsonInventory::Color &x)
    {
        switch (x)
        {
        case JsonInventory::Color::B0_C3_D9:
            j = "b0c3d9";
            break;
        case JsonInventory::Color::CF6_A32:
            j = "CF6A32";
            break;
        case JsonInventory::Color::D32_CE6:
            j = "d32ce6";
            break;
        case JsonInventory::Color::D83636:
            j = "d83636";
            break;
        case JsonInventory::Color::E1_E10_F:
            j = "e1e10f";
            break;
        case JsonInventory::Color::EB4_B4_B:
            j = "eb4b4b";
            break;
        case JsonInventory::Color::FFD700:
            j = "ffd700";
            break;
        case JsonInventory::Color::THE_00_A000:
            j = "00a000";
            break;
        case JsonInventory::Color::THE_4_B69_FF:
            j = "4b69ff";
            break;
        case JsonInventory::Color::THE_5_E98_D9:
            j = "5e98d9";
            break;
        case JsonInventory::Color::THE_6_F6_A63:
            j = "6F6A63";
            break;
        case JsonInventory::Color::THE_756_B5_E:
            j = "756b5e";
            break;
        case JsonInventory::Color::THE_7_D6_D00:
            j = "7D6D00";
            break;
        case JsonInventory::Color::THE_7_EA9_D1:
            j = "7ea9d1";
            break;
        case JsonInventory::Color::THE_8847_FF:
            j = "8847ff";
            break;
        case JsonInventory::Color::THE_8_B8989:
            j = "8b8989";
            break;
        default:
            throw "This should not happen";
        }
    }

    inline void from_json(const json &j, JsonInventory::Category &x)
    {
        if (j == "Class")
            x = JsonInventory::Category::CLASS;
        else if (j == "Collection")
            x = JsonInventory::Category::COLLECTION;
        else if (j == "Exterior")
            x = JsonInventory::Category::EXTERIOR;
        else if (j == "Grade")
            x = JsonInventory::Category::GRADE;
        else if (j == "Quality")
            x = JsonInventory::Category::QUALITY;
        else if (j == "Rarity")
            x = JsonInventory::Category::RARITY;
        else if (j == "Type")
            x = JsonInventory::Category::TYPE;
        else
            throw "Input JSON does not conform to schema";
    }

    inline void to_json(json &j, const JsonInventory::Category &x)
    {
        switch (x)
        {
        case JsonInventory::Category::CLASS:
            j = "Class";
            break;
        case JsonInventory::Category::COLLECTION:
            j = "Collection";
            break;
        case JsonInventory::Category::EXTERIOR:
            j = "Exterior";
            break;
        case JsonInventory::Category::GRADE:
            j = "Grade";
            break;
        case JsonInventory::Category::QUALITY:
            j = "Quality";
            break;
        case JsonInventory::Category::RARITY:
            j = "Rarity";
            break;
        case JsonInventory::Category::TYPE:
            j = "Type";
            break;
        default:
            throw "This should not happen";
        }
    }

    inline void from_json(const json &j, JsonInventory::InternalName &x)
    {
        static std::unordered_map<std::string, JsonInventory::InternalName> enumValues{
            {"action", JsonInventory::InternalName::ACTION},
            {"BlueMoonCosmetics_collection", JsonInventory::InternalName::BLUE_MOON_COSMETICS_COLLECTION},
            {"building", JsonInventory::InternalName::BUILDING},
            {"Campaign3Cosmetics_Case1_collection", JsonInventory::InternalName::CAMPAIGN3_COSMETICS_CASE1_COLLECTION},
            {"Campaign3Cosmetics_Case2_collection", JsonInventory::InternalName::CAMPAIGN3_COSMETICS_CASE2_COLLECTION},
            {"campaign3_master_collection", JsonInventory::InternalName::CAMPAIGN3_MASTER_COLLECTION},
            {"Campaign3Paintkits_case1_collection", JsonInventory::InternalName::CAMPAIGN3_PAINTKITS_CASE1_COLLECTION},
            {"Campaign3Paintkits_contract1_collection", JsonInventory::InternalName::CAMPAIGN3_PAINTKITS_CONTRACT1_COLLECTION},
            {"Campaign3Paintkits_contract2_collection", JsonInventory::InternalName::CAMPAIGN3_PAINTKITS_CONTRACT2_COLLECTION},
            {"collectors", JsonInventory::InternalName::COLLECTORS},
            {"community", JsonInventory::InternalName::COMMUNITY},
            {"concealedkiller_collection", JsonInventory::InternalName::CONCEALEDKILLER_COLLECTION},
            {"craftsmann_collection", JsonInventory::InternalName::CRAFTSMANN_COLLECTION},
            {"Craft Item", JsonInventory::InternalName::CRAFT_ITEM},
            {"Demoman", JsonInventory::InternalName::DEMOMAN},
            {"", JsonInventory::InternalName::EMPTY},
            {"Engineer", JsonInventory::InternalName::ENGINEER},
            {"GunMettleCosmetics_collection", JsonInventory::InternalName::GUN_METTLE_COSMETICS_COLLECTION},
            {"halloween2015_collection_name", JsonInventory::InternalName::HALLOWEEN2015_COLLECTION_NAME},
            {"halloween2016_collection_name", JsonInventory::InternalName::HALLOWEEN2016_COLLECTION_NAME},
            {"halloween2018_collection_name", JsonInventory::InternalName::HALLOWEEN2018_COLLECTION_NAME},
            {"Halloween2018Paintkits_collection", JsonInventory::InternalName::HALLOWEEN2018_PAINTKITS_COLLECTION},
            {"halloween2019_collection_name", JsonInventory::InternalName::HALLOWEEN2019_COLLECTION_NAME},
            {"halloween2020_collection_name", JsonInventory::InternalName::HALLOWEEN2020_COLLECTION_NAME},
            {"Halloween2020Paintkits_collection", JsonInventory::InternalName::HALLOWEEN2020_PAINTKITS_COLLECTION},
            {"halloween2021_collection_name", JsonInventory::InternalName::HALLOWEEN2021_COLLECTION_NAME},
            {"Halloween2021Paintkits_collection", JsonInventory::InternalName::HALLOWEEN2021_PAINTKITS_COLLECTION},
            {"Halloween_master_collection", JsonInventory::InternalName::HALLOWEEN_MASTER_COLLECTION},
            {"haunted", JsonInventory::InternalName::HAUNTED},
            {"Heavy", JsonInventory::InternalName::HEAVY},
            {"TF_GIFT", JsonInventory::InternalName::INTERNAL_NAME_TF_GIFT},
            {"Invasion_collection_01", JsonInventory::InternalName::INVASION_COLLECTION_01},
            {"Invasion_collection_02", JsonInventory::InternalName::INVASION_COLLECTION_02},
            {"invasion_master_collection", JsonInventory::InternalName::INVASION_MASTER_COLLECTION},
            {"MayflowerCosmetics_collection", JsonInventory::InternalName::MAYFLOWER_COSMETICS_COLLECTION},
            {"Medic", JsonInventory::InternalName::MEDIC},
            {"melee", JsonInventory::InternalName::MELEE},
            {"misc", JsonInventory::InternalName::MISC},
            {"Normal", JsonInventory::InternalName::NORMAL},
            {"operation_gun_mettle_master_collection", JsonInventory::InternalName::OPERATION_GUN_METTLE_MASTER_COLLECTION},
            {"operation_tough_break_master_collection", JsonInventory::InternalName::OPERATION_TOUGH_BREAK_MASTER_COLLECTION},
            {"paintkitweapon", JsonInventory::InternalName::PAINTKITWEAPON},
            {"pda", JsonInventory::InternalName::PDA},
            {"pda2", JsonInventory::InternalName::PDA2},
            {"Powerhouse_collection", JsonInventory::InternalName::POWERHOUSE_COLLECTION},
            {"primary", JsonInventory::InternalName::PRIMARY},
            {"Pyro", JsonInventory::InternalName::PYRO},
            {"RainyDayCosmetics_collection", JsonInventory::InternalName::RAINY_DAY_COSMETICS_COLLECTION},
            {"rarity1", JsonInventory::InternalName::RARITY1},
            {"rarity4", JsonInventory::InternalName::RARITY4},
            {"Rarity_Ancient", JsonInventory::InternalName::RARITY_ANCIENT},
            {"Rarity_Common", JsonInventory::InternalName::RARITY_COMMON},
            {"Rarity_Legendary", JsonInventory::InternalName::RARITY_LEGENDARY},
            {"Rarity_Mythical", JsonInventory::InternalName::RARITY_MYTHICAL},
            {"Rarity_Rare", JsonInventory::InternalName::RARITY_RARE},
            {"Rarity_Uncommon", JsonInventory::InternalName::RARITY_UNCOMMON},
            {"Scout", JsonInventory::InternalName::SCOUT},
            {"secondary", JsonInventory::InternalName::SECONDARY},
            {"selfmade", JsonInventory::InternalName::SELFMADE},
            {"Sniper", JsonInventory::InternalName::SNIPER},
            {"Soldier", JsonInventory::InternalName::SOLDIER},
            {"Spy", JsonInventory::InternalName::SPY},
            {"strange", JsonInventory::InternalName::STRANGE},
            {"Summer2019Cosmetics_collection", JsonInventory::InternalName::SUMMER2019_COSMETICS_COLLECTION},
            {"Summer2020Cosmetics_collection", JsonInventory::InternalName::SUMMER2020_COSMETICS_COLLECTION},
            {"Summer2021Cosmetics_collection", JsonInventory::InternalName::SUMMER2021_COSMETICS_COLLECTION},
            {"Supply Crate", JsonInventory::InternalName::SUPPLY_CRATE},
            {"taunt", JsonInventory::InternalName::TAUNT},
            {"teufort_collection", JsonInventory::InternalName::TEUFORT_COLLECTION},
            {"TFUI_InvTooltip_BattleScared", JsonInventory::InternalName::TFUI_INV_TOOLTIP_BATTLE_SCARED},
            {"TFUI_InvTooltip_FactoryNew", JsonInventory::InternalName::TFUI_INV_TOOLTIP_FACTORY_NEW},
            {"TFUI_InvTooltip_FieldTested", JsonInventory::InternalName::TFUI_INV_TOOLTIP_FIELD_TESTED},
            {"TFUI_InvTooltip_MinimalWear", JsonInventory::InternalName::TFUI_INV_TOOLTIP_MINIMAL_WEAR},
            {"TFUI_InvTooltip_WellWorn", JsonInventory::InternalName::TFUI_INV_TOOLTIP_WELL_WORN},
            {"TF_FestivizerTool", JsonInventory::InternalName::TF_FESTIVIZER_TOOL},
            {"TF_Gift", JsonInventory::InternalName::TF_GIFT},
            {"TF_ItemDynamicRecipeTool", JsonInventory::InternalName::TF_ITEM_DYNAMIC_RECIPE_TOOL},
            {"TF_KillStreakifierToolA", JsonInventory::InternalName::TF_KILL_STREAKIFIER_TOOL_A},
            {"TF_KillStreakifierToolC", JsonInventory::InternalName::TF_KILL_STREAKIFIER_TOOL_C},
            {"TF_LockedCrate", JsonInventory::InternalName::TF_LOCKED_CRATE},
            {"TF_Package_Type", JsonInventory::InternalName::TF_PACKAGE_TYPE},
            {"TF_PaintKitTool", JsonInventory::InternalName::TF_PAINT_KIT_TOOL},
            {"TF_Party_Favor", JsonInventory::InternalName::TF_PARTY_FAVOR},
            {"TF_ServerEnchantmentType", JsonInventory::InternalName::TF_SERVER_ENCHANTMENT_TYPE},
            {"TF_SpellbookPage_Type", JsonInventory::InternalName::TF_SPELLBOOK_PAGE_TYPE},
            {"TF_StrangePart", JsonInventory::InternalName::TF_STRANGE_PART},
            {"TF_StrangifierTool", JsonInventory::InternalName::TF_STRANGIFIER_TOOL},
            {"TF_T", JsonInventory::InternalName::TF_T},
            {"TF_Unusualifier", JsonInventory::InternalName::TF_UNUSUALIFIER},
            {"TF_UsableItem", JsonInventory::InternalName::TF_USABLE_ITEM},
            {"tough_break_case_collection_01", JsonInventory::InternalName::TOUGH_BREAK_CASE_COLLECTION_01},
            {"ToughBreakCosmetics_collection", JsonInventory::InternalName::TOUGH_BREAK_COSMETICS_COLLECTION},
            {"tough_break_drop_collection_01", JsonInventory::InternalName::TOUGH_BREAK_DROP_COLLECTION_01},
            {"tough_break_drop_collection_02", JsonInventory::InternalName::TOUGH_BREAK_DROP_COLLECTION_02},
            {"Unique", JsonInventory::InternalName::UNIQUE},
            {"UniqueFreeSkins_collection", JsonInventory::InternalName::UNIQUE_FREE_SKINS_COLLECTION},
            {"vintage", JsonInventory::InternalName::VINTAGE},
            {"Winter2016Cosmetics_collection", JsonInventory::InternalName::WINTER2016_COSMETICS_COLLECTION},
            {"Winter2017Cosmetics_collection", JsonInventory::InternalName::WINTER2017_COSMETICS_COLLECTION},
            {"Winter2018Cosmetics_collection", JsonInventory::InternalName::WINTER2018_COSMETICS_COLLECTION},
            {"Winter2019Cosmetics_collection", JsonInventory::InternalName::WINTER2019_COSMETICS_COLLECTION},
            {"Winter2019Paintkits_collection", JsonInventory::InternalName::WINTER2019_PAINTKITS_COLLECTION},
            {"Winter2020Cosmetics_collection", JsonInventory::InternalName::WINTER2020_COSMETICS_COLLECTION},
            {"Winter2020Paintkits_collection", JsonInventory::InternalName::WINTER2020_PAINTKITS_COLLECTION},
            {"Winter2021Cosmetics_collection", JsonInventory::InternalName::WINTER2021_COSMETICS_COLLECTION},
        };
        auto iter = enumValues.find(j.get<std::string>());
        if (iter != enumValues.end())
        {
            x = iter->second;
        }
    }

    inline void to_json(json &j, const JsonInventory::InternalName &x)
    {
        switch (x)
        {
        case JsonInventory::InternalName::ACTION:
            j = "action";
            break;
        case JsonInventory::InternalName::BLUE_MOON_COSMETICS_COLLECTION:
            j = "BlueMoonCosmetics_collection";
            break;
        case JsonInventory::InternalName::BUILDING:
            j = "building";
            break;
        case JsonInventory::InternalName::CAMPAIGN3_COSMETICS_CASE1_COLLECTION:
            j = "Campaign3Cosmetics_Case1_collection";
            break;
        case JsonInventory::InternalName::CAMPAIGN3_COSMETICS_CASE2_COLLECTION:
            j = "Campaign3Cosmetics_Case2_collection";
            break;
        case JsonInventory::InternalName::CAMPAIGN3_MASTER_COLLECTION:
            j = "campaign3_master_collection";
            break;
        case JsonInventory::InternalName::CAMPAIGN3_PAINTKITS_CASE1_COLLECTION:
            j = "Campaign3Paintkits_case1_collection";
            break;
        case JsonInventory::InternalName::CAMPAIGN3_PAINTKITS_CONTRACT1_COLLECTION:
            j = "Campaign3Paintkits_contract1_collection";
            break;
        case JsonInventory::InternalName::CAMPAIGN3_PAINTKITS_CONTRACT2_COLLECTION:
            j = "Campaign3Paintkits_contract2_collection";
            break;
        case JsonInventory::InternalName::COLLECTORS:
            j = "collectors";
            break;
        case JsonInventory::InternalName::COMMUNITY:
            j = "community";
            break;
        case JsonInventory::InternalName::CONCEALEDKILLER_COLLECTION:
            j = "concealedkiller_collection";
            break;
        case JsonInventory::InternalName::CRAFTSMANN_COLLECTION:
            j = "craftsmann_collection";
            break;
        case JsonInventory::InternalName::CRAFT_ITEM:
            j = "Craft Item";
            break;
        case JsonInventory::InternalName::DEMOMAN:
            j = "Demoman";
            break;
        case JsonInventory::InternalName::EMPTY:
            j = "";
            break;
        case JsonInventory::InternalName::ENGINEER:
            j = "Engineer";
            break;
        case JsonInventory::InternalName::GUN_METTLE_COSMETICS_COLLECTION:
            j = "GunMettleCosmetics_collection";
            break;
        case JsonInventory::InternalName::HALLOWEEN2015_COLLECTION_NAME:
            j = "halloween2015_collection_name";
            break;
        case JsonInventory::InternalName::HALLOWEEN2016_COLLECTION_NAME:
            j = "halloween2016_collection_name";
            break;
        case JsonInventory::InternalName::HALLOWEEN2018_COLLECTION_NAME:
            j = "halloween2018_collection_name";
            break;
        case JsonInventory::InternalName::HALLOWEEN2018_PAINTKITS_COLLECTION:
            j = "Halloween2018Paintkits_collection";
            break;
        case JsonInventory::InternalName::HALLOWEEN2019_COLLECTION_NAME:
            j = "halloween2019_collection_name";
            break;
        case JsonInventory::InternalName::HALLOWEEN2020_COLLECTION_NAME:
            j = "halloween2020_collection_name";
            break;
        case JsonInventory::InternalName::HALLOWEEN2020_PAINTKITS_COLLECTION:
            j = "Halloween2020Paintkits_collection";
            break;
        case JsonInventory::InternalName::HALLOWEEN2021_COLLECTION_NAME:
            j = "halloween2021_collection_name";
            break;
        case JsonInventory::InternalName::HALLOWEEN2021_PAINTKITS_COLLECTION:
            j = "Halloween2021Paintkits_collection";
            break;
        case JsonInventory::InternalName::HALLOWEEN_MASTER_COLLECTION:
            j = "Halloween_master_collection";
            break;
        case JsonInventory::InternalName::HAUNTED:
            j = "haunted";
            break;
        case JsonInventory::InternalName::HEAVY:
            j = "Heavy";
            break;
        case JsonInventory::InternalName::INTERNAL_NAME_TF_GIFT:
            j = "TF_GIFT";
            break;
        case JsonInventory::InternalName::INVASION_COLLECTION_01:
            j = "Invasion_collection_01";
            break;
        case JsonInventory::InternalName::INVASION_COLLECTION_02:
            j = "Invasion_collection_02";
            break;
        case JsonInventory::InternalName::INVASION_MASTER_COLLECTION:
            j = "invasion_master_collection";
            break;
        case JsonInventory::InternalName::MAYFLOWER_COSMETICS_COLLECTION:
            j = "MayflowerCosmetics_collection";
            break;
        case JsonInventory::InternalName::MEDIC:
            j = "Medic";
            break;
        case JsonInventory::InternalName::MELEE:
            j = "melee";
            break;
        case JsonInventory::InternalName::MISC:
            j = "misc";
            break;
        case JsonInventory::InternalName::NORMAL:
            j = "Normal";
            break;
        case JsonInventory::InternalName::OPERATION_GUN_METTLE_MASTER_COLLECTION:
            j = "operation_gun_mettle_master_collection";
            break;
        case JsonInventory::InternalName::OPERATION_TOUGH_BREAK_MASTER_COLLECTION:
            j = "operation_tough_break_master_collection";
            break;
        case JsonInventory::InternalName::PAINTKITWEAPON:
            j = "paintkitweapon";
            break;
        case JsonInventory::InternalName::PDA:
            j = "pda";
            break;
        case JsonInventory::InternalName::PDA2:
            j = "pda2";
            break;
        case JsonInventory::InternalName::POWERHOUSE_COLLECTION:
            j = "Powerhouse_collection";
            break;
        case JsonInventory::InternalName::PRIMARY:
            j = "primary";
            break;
        case JsonInventory::InternalName::PYRO:
            j = "Pyro";
            break;
        case JsonInventory::InternalName::RAINY_DAY_COSMETICS_COLLECTION:
            j = "RainyDayCosmetics_collection";
            break;
        case JsonInventory::InternalName::RARITY1:
            j = "rarity1";
            break;
        case JsonInventory::InternalName::RARITY4:
            j = "rarity4";
            break;
        case JsonInventory::InternalName::RARITY_ANCIENT:
            j = "Rarity_Ancient";
            break;
        case JsonInventory::InternalName::RARITY_COMMON:
            j = "Rarity_Common";
            break;
        case JsonInventory::InternalName::RARITY_LEGENDARY:
            j = "Rarity_Legendary";
            break;
        case JsonInventory::InternalName::RARITY_MYTHICAL:
            j = "Rarity_Mythical";
            break;
        case JsonInventory::InternalName::RARITY_RARE:
            j = "Rarity_Rare";
            break;
        case JsonInventory::InternalName::RARITY_UNCOMMON:
            j = "Rarity_Uncommon";
            break;
        case JsonInventory::InternalName::SCOUT:
            j = "Scout";
            break;
        case JsonInventory::InternalName::SECONDARY:
            j = "secondary";
            break;
        case JsonInventory::InternalName::SELFMADE:
            j = "selfmade";
            break;
        case JsonInventory::InternalName::SNIPER:
            j = "Sniper";
            break;
        case JsonInventory::InternalName::SOLDIER:
            j = "Soldier";
            break;
        case JsonInventory::InternalName::SPY:
            j = "Spy";
            break;
        case JsonInventory::InternalName::STRANGE:
            j = "strange";
            break;
        case JsonInventory::InternalName::SUMMER2019_COSMETICS_COLLECTION:
            j = "Summer2019Cosmetics_collection";
            break;
        case JsonInventory::InternalName::SUMMER2020_COSMETICS_COLLECTION:
            j = "Summer2020Cosmetics_collection";
            break;
        case JsonInventory::InternalName::SUMMER2021_COSMETICS_COLLECTION:
            j = "Summer2021Cosmetics_collection";
            break;
        case JsonInventory::InternalName::SUPPLY_CRATE:
            j = "Supply Crate";
            break;
        case JsonInventory::InternalName::TAUNT:
            j = "taunt";
            break;
        case JsonInventory::InternalName::TEUFORT_COLLECTION:
            j = "teufort_collection";
            break;
        case JsonInventory::InternalName::TFUI_INV_TOOLTIP_BATTLE_SCARED:
            j = "TFUI_InvTooltip_BattleScared";
            break;
        case JsonInventory::InternalName::TFUI_INV_TOOLTIP_FACTORY_NEW:
            j = "TFUI_InvTooltip_FactoryNew";
            break;
        case JsonInventory::InternalName::TFUI_INV_TOOLTIP_FIELD_TESTED:
            j = "TFUI_InvTooltip_FieldTested";
            break;
        case JsonInventory::InternalName::TFUI_INV_TOOLTIP_MINIMAL_WEAR:
            j = "TFUI_InvTooltip_MinimalWear";
            break;
        case JsonInventory::InternalName::TFUI_INV_TOOLTIP_WELL_WORN:
            j = "TFUI_InvTooltip_WellWorn";
            break;
        case JsonInventory::InternalName::TF_FESTIVIZER_TOOL:
            j = "TF_FestivizerTool";
            break;
        case JsonInventory::InternalName::TF_GIFT:
            j = "TF_Gift";
            break;
        case JsonInventory::InternalName::TF_ITEM_DYNAMIC_RECIPE_TOOL:
            j = "TF_ItemDynamicRecipeTool";
            break;
        case JsonInventory::InternalName::TF_KILL_STREAKIFIER_TOOL_A:
            j = "TF_KillStreakifierToolA";
            break;
        case JsonInventory::InternalName::TF_KILL_STREAKIFIER_TOOL_C:
            j = "TF_KillStreakifierToolC";
            break;
        case JsonInventory::InternalName::TF_LOCKED_CRATE:
            j = "TF_LockedCrate";
            break;
        case JsonInventory::InternalName::TF_PACKAGE_TYPE:
            j = "TF_Package_Type";
            break;
        case JsonInventory::InternalName::TF_PAINT_KIT_TOOL:
            j = "TF_PaintKitTool";
            break;
        case JsonInventory::InternalName::TF_PARTY_FAVOR:
            j = "TF_Party_Favor";
            break;
        case JsonInventory::InternalName::TF_SERVER_ENCHANTMENT_TYPE:
            j = "TF_ServerEnchantmentType";
            break;
        case JsonInventory::InternalName::TF_SPELLBOOK_PAGE_TYPE:
            j = "TF_SpellbookPage_Type";
            break;
        case JsonInventory::InternalName::TF_STRANGE_PART:
            j = "TF_StrangePart";
            break;
        case JsonInventory::InternalName::TF_STRANGIFIER_TOOL:
            j = "TF_StrangifierTool";
            break;
        case JsonInventory::InternalName::TF_T:
            j = "TF_T";
            break;
        case JsonInventory::InternalName::TF_UNUSUALIFIER:
            j = "TF_Unusualifier";
            break;
        case JsonInventory::InternalName::TF_USABLE_ITEM:
            j = "TF_UsableItem";
            break;
        case JsonInventory::InternalName::TOUGH_BREAK_CASE_COLLECTION_01:
            j = "tough_break_case_collection_01";
            break;
        case JsonInventory::InternalName::TOUGH_BREAK_COSMETICS_COLLECTION:
            j = "ToughBreakCosmetics_collection";
            break;
        case JsonInventory::InternalName::TOUGH_BREAK_DROP_COLLECTION_01:
            j = "tough_break_drop_collection_01";
            break;
        case JsonInventory::InternalName::TOUGH_BREAK_DROP_COLLECTION_02:
            j = "tough_break_drop_collection_02";
            break;
        case JsonInventory::InternalName::UNIQUE:
            j = "Unique";
            break;
        case JsonInventory::InternalName::UNIQUE_FREE_SKINS_COLLECTION:
            j = "UniqueFreeSkins_collection";
            break;
        case JsonInventory::InternalName::VINTAGE:
            j = "vintage";
            break;
        case JsonInventory::InternalName::WINTER2016_COSMETICS_COLLECTION:
            j = "Winter2016Cosmetics_collection";
            break;
        case JsonInventory::InternalName::WINTER2017_COSMETICS_COLLECTION:
            j = "Winter2017Cosmetics_collection";
            break;
        case JsonInventory::InternalName::WINTER2018_COSMETICS_COLLECTION:
            j = "Winter2018Cosmetics_collection";
            break;
        case JsonInventory::InternalName::WINTER2019_COSMETICS_COLLECTION:
            j = "Winter2019Cosmetics_collection";
            break;
        case JsonInventory::InternalName::WINTER2019_PAINTKITS_COLLECTION:
            j = "Winter2019Paintkits_collection";
            break;
        case JsonInventory::InternalName::WINTER2020_COSMETICS_COLLECTION:
            j = "Winter2020Cosmetics_collection";
            break;
        case JsonInventory::InternalName::WINTER2020_PAINTKITS_COLLECTION:
            j = "Winter2020Paintkits_collection";
            break;
        case JsonInventory::InternalName::WINTER2021_COSMETICS_COLLECTION:
            j = "Winter2021Cosmetics_collection";
            break;
        default:
            throw "This should not happen";
        }
    }

    inline void from_json(const json &j, JsonInventory::LocalizedTagName &x)
    {
        static std::unordered_map<std::string, JsonInventory::LocalizedTagName> enumValues{
            {"Abominable Cosmetic Collection", JsonInventory::LocalizedTagName::ABOMINABLE_COSMETIC_COLLECTION},
            {"Action", JsonInventory::LocalizedTagName::ACTION},
            {"Assassin", JsonInventory::LocalizedTagName::ASSASSIN},
            {"Battle Scarred", JsonInventory::LocalizedTagName::BATTLE_SCARRED},
            {"Blue Moon Cosmetics Collection", JsonInventory::LocalizedTagName::BLUE_MOON_COSMETICS_COLLECTION},
            {"Building", JsonInventory::LocalizedTagName::BUILDING},
            {"Civilian", JsonInventory::LocalizedTagName::CIVILIAN},
            {"Collector's", JsonInventory::LocalizedTagName::COLLECTOR_S},
            {"Commando", JsonInventory::LocalizedTagName::COMMANDO},
            {"Community", JsonInventory::LocalizedTagName::COMMUNITY},
            {"Concealed Killer Collection", JsonInventory::LocalizedTagName::CONCEALED_KILLER_COLLECTION},
            {"Confidential Collection Case", JsonInventory::LocalizedTagName::CONFIDENTIAL_COLLECTION_CASE},
            {"Contract Campaigner Collection", JsonInventory::LocalizedTagName::CONTRACT_CAMPAIGNER_COLLECTION},
            {"Cosmetic", JsonInventory::LocalizedTagName::COSMETIC},
            {"Craftsmann Collection", JsonInventory::LocalizedTagName::CRAFTSMANN_COLLECTION},
            {"Craft Item", JsonInventory::LocalizedTagName::CRAFT_ITEM},
            {"Crate", JsonInventory::LocalizedTagName::CRATE},
            {"Creepy Crawly Collection", JsonInventory::LocalizedTagName::CREEPY_CRAWLY_COLLECTION},
            {"Crimson Cache Collection", JsonInventory::LocalizedTagName::CRIMSON_CACHE_COLLECTION},
            {"Decorated War Hero Collection", JsonInventory::LocalizedTagName::DECORATED_WAR_HERO_COLLECTION},
            {"Decorated Weapon", JsonInventory::LocalizedTagName::DECORATED_WEAPON},
            {"Demoman", JsonInventory::LocalizedTagName::DEMOMAN},
            {"Elite", JsonInventory::LocalizedTagName::ELITE},
            {"", JsonInventory::LocalizedTagName::EMPTY},
            {"Engineer", JsonInventory::LocalizedTagName::ENGINEER},
            {"Factory New", JsonInventory::LocalizedTagName::FACTORY_NEW},
            {"Festivizer", JsonInventory::LocalizedTagName::FESTIVIZER},
            {"Field-Tested", JsonInventory::LocalizedTagName::FIELD_TESTED},
            {"Freelance", JsonInventory::LocalizedTagName::FREELANCE},
            {"Gargoyle Collection", JsonInventory::LocalizedTagName::GARGOYLE_COLLECTION},
            {"Gentlemanne's Collection", JsonInventory::LocalizedTagName::GENTLEMANNE_S_COLLECTION},
            {"Genuine", JsonInventory::LocalizedTagName::GENUINE},
            {"Gift", JsonInventory::LocalizedTagName::GIFT},
            {"Gun Mettle Cosmetics Collection", JsonInventory::LocalizedTagName::GUN_METTLE_COSMETICS_COLLECTION},
            {"Harvest Collection", JsonInventory::LocalizedTagName::HARVEST_COLLECTION},
            {"Haunted", JsonInventory::LocalizedTagName::HAUNTED},
            {"Heavy", JsonInventory::LocalizedTagName::HEAVY},
            {"Invasion Collections", JsonInventory::LocalizedTagName::INVASION_COLLECTIONS},
            {"Jungle Inferno Collections", JsonInventory::LocalizedTagName::JUNGLE_INFERNO_COLLECTIONS},
            {"Jungle Jackpot Collection", JsonInventory::LocalizedTagName::JUNGLE_JACKPOT_COLLECTION},
            {"Killstreak Kit", JsonInventory::LocalizedTagName::KILLSTREAK_KIT},
            {"Mann Co. Events Collection", JsonInventory::LocalizedTagName::MANN_CO_EVENTS_COLLECTION},
            {"Mayflower Cosmetics Collection", JsonInventory::LocalizedTagName::MAYFLOWER_COSMETICS_COLLECTION},
            {"Medic", JsonInventory::LocalizedTagName::MEDIC},
            {"Melee weapon", JsonInventory::LocalizedTagName::MELEE_WEAPON},
            {"Mercenary", JsonInventory::LocalizedTagName::MERCENARY},
            {"Minimal Wear", JsonInventory::LocalizedTagName::MINIMAL_WEAR},
            {"Normal", JsonInventory::LocalizedTagName::NORMAL},
            {"Package", JsonInventory::LocalizedTagName::PACKAGE},
            {"Party Favor", JsonInventory::LocalizedTagName::PARTY_FAVOR},
            {"Powerhouse Collection", JsonInventory::LocalizedTagName::POWERHOUSE_COLLECTION},
            {"Primary PDA", JsonInventory::LocalizedTagName::PRIMARY_PDA},
            {"Primary weapon", JsonInventory::LocalizedTagName::PRIMARY_WEAPON},
            {"Professional Killstreak Kit", JsonInventory::LocalizedTagName::PROFESSIONAL_KILLSTREAK_KIT},
            {"Pyro", JsonInventory::LocalizedTagName::PYRO},
            {"Pyroland Collection", JsonInventory::LocalizedTagName::PYROLAND_COLLECTION},
            {"Quarantined Collection Case", JsonInventory::LocalizedTagName::QUARANTINED_COLLECTION_CASE},
            {"Rainy Day Cosmetics Collection", JsonInventory::LocalizedTagName::RAINY_DAY_COSMETICS_COLLECTION},
            {"Recipe", JsonInventory::LocalizedTagName::RECIPE},
            {"Scout", JsonInventory::LocalizedTagName::SCOUT},
            {"Scream Fortress XIII Collection", JsonInventory::LocalizedTagName::SCREAM_FORTRESS_XIII_COLLECTION},
            {"Scream Fortress XII Collection", JsonInventory::LocalizedTagName::SCREAM_FORTRESS_XII_COLLECTION},
            {"Scream Fortress X Collection", JsonInventory::LocalizedTagName::SCREAM_FORTRESS_X_COLLECTION},
            {"Secondary PDA", JsonInventory::LocalizedTagName::SECONDARY_PDA},
            {"Secondary weapon", JsonInventory::LocalizedTagName::SECONDARY_WEAPON},
            {"Self-Made", JsonInventory::LocalizedTagName::SELF_MADE},
            {"Server Enchantment", JsonInventory::LocalizedTagName::SERVER_ENCHANTMENT},
            {"Sniper", JsonInventory::LocalizedTagName::SNIPER},
            {"Soldier", JsonInventory::LocalizedTagName::SOLDIER},
            {"Spellbook Page", JsonInventory::LocalizedTagName::SPELLBOOK_PAGE},
            {"Spooky Spoils Collection", JsonInventory::LocalizedTagName::SPOOKY_SPOILS_COLLECTION},
            {"Spy", JsonInventory::LocalizedTagName::SPY},
            {"Strange", JsonInventory::LocalizedTagName::STRANGE},
            {"Strange Part", JsonInventory::LocalizedTagName::STRANGE_PART},
            {"Strangifier", JsonInventory::LocalizedTagName::STRANGIFIER},
            {"Summer 2019 Cosmetics Collection", JsonInventory::LocalizedTagName::SUMMER_2019_COSMETICS_COLLECTION},
            {"Summer 2020 Cosmetics Collection", JsonInventory::LocalizedTagName::SUMMER_2020_COSMETICS_COLLECTION},
            {"Summer 2021 Cosmetics Collection", JsonInventory::LocalizedTagName::SUMMER_2021_COSMETICS_COLLECTION},
            {"Supply Crate", JsonInventory::LocalizedTagName::SUPPLY_CRATE},
            {"Taunt 1", JsonInventory::LocalizedTagName::TAUNT_1},
            {"Teufort Collection", JsonInventory::LocalizedTagName::TEUFORT_COLLECTION},
            {"The Gun Mettle Collections", JsonInventory::LocalizedTagName::THE_GUN_METTLE_COLLECTIONS},
            {"The Scream Fortress Collections", JsonInventory::LocalizedTagName::THE_SCREAM_FORTRESS_COLLECTIONS},
            {"The Tough Break Collections", JsonInventory::LocalizedTagName::THE_TOUGH_BREAK_COLLECTIONS},
            {"Tool", JsonInventory::LocalizedTagName::TOOL},
            {"Tough Break Cosmetics Collection", JsonInventory::LocalizedTagName::TOUGH_BREAK_COSMETICS_COLLECTION},
            {"Unique", JsonInventory::LocalizedTagName::UNIQUE},
            {"Unleash the Beast Cosmetic Collection", JsonInventory::LocalizedTagName::UNLEASH_THE_BEAST_COSMETIC_COLLECTION},
            {"Unusual", JsonInventory::LocalizedTagName::UNUSUAL},
            {"Unusualifier", JsonInventory::LocalizedTagName::UNUSUALIFIER},
            {"Usable Item", JsonInventory::LocalizedTagName::USABLE_ITEM},
            {"Vintage", JsonInventory::LocalizedTagName::VINTAGE},
            {"Violet Vermin Collection", JsonInventory::LocalizedTagName::VIOLET_VERMIN_COLLECTION},
            {"War Paint", JsonInventory::LocalizedTagName::WAR_PAINT},
            {"Well-Worn", JsonInventory::LocalizedTagName::WELL_WORN},
            {"Wicked Windfall Collection", JsonInventory::LocalizedTagName::WICKED_WINDFALL_COLLECTION},
            {"Winter 2016 Cosmetics Collection", JsonInventory::LocalizedTagName::WINTER_2016_COSMETICS_COLLECTION},
            {"Winter 2017 Cosmetics Collection", JsonInventory::LocalizedTagName::WINTER_2017_COSMETICS_COLLECTION},
            {"Winter 2018 Cosmetics Collection", JsonInventory::LocalizedTagName::WINTER_2018_COSMETICS_COLLECTION},
            {"Winter 2019 Collection", JsonInventory::LocalizedTagName::WINTER_2019_COLLECTION},
            {"Winter 2019 Cosmetics Collection", JsonInventory::LocalizedTagName::WINTER_2019_COSMETICS_COLLECTION},
            {"Winter 2020 Collection", JsonInventory::LocalizedTagName::WINTER_2020_COLLECTION},
            {"Winter 2020 Cosmetics Collection", JsonInventory::LocalizedTagName::WINTER_2020_COSMETICS_COLLECTION},
            {"Winter 2021 Cosmetics Collection", JsonInventory::LocalizedTagName::WINTER_2021_COSMETICS_COLLECTION},
        };
        auto iter = enumValues.find(j.get<std::string>());
        if (iter != enumValues.end())
        {
            x = iter->second;
        }
    }

    inline void to_json(json &j, const JsonInventory::LocalizedTagName &x)
    {
        switch (x)
        {
        case JsonInventory::LocalizedTagName::ABOMINABLE_COSMETIC_COLLECTION:
            j = "Abominable Cosmetic Collection";
            break;
        case JsonInventory::LocalizedTagName::ACTION:
            j = "Action";
            break;
        case JsonInventory::LocalizedTagName::ASSASSIN:
            j = "Assassin";
            break;
        case JsonInventory::LocalizedTagName::BATTLE_SCARRED:
            j = "Battle Scarred";
            break;
        case JsonInventory::LocalizedTagName::BLUE_MOON_COSMETICS_COLLECTION:
            j = "Blue Moon Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::BUILDING:
            j = "Building";
            break;
        case JsonInventory::LocalizedTagName::CIVILIAN:
            j = "Civilian";
            break;
        case JsonInventory::LocalizedTagName::COLLECTOR_S:
            j = "Collector's";
            break;
        case JsonInventory::LocalizedTagName::COMMANDO:
            j = "Commando";
            break;
        case JsonInventory::LocalizedTagName::COMMUNITY:
            j = "Community";
            break;
        case JsonInventory::LocalizedTagName::CONCEALED_KILLER_COLLECTION:
            j = "Concealed Killer Collection";
            break;
        case JsonInventory::LocalizedTagName::CONFIDENTIAL_COLLECTION_CASE:
            j = "Confidential Collection Case";
            break;
        case JsonInventory::LocalizedTagName::CONTRACT_CAMPAIGNER_COLLECTION:
            j = "Contract Campaigner Collection";
            break;
        case JsonInventory::LocalizedTagName::COSMETIC:
            j = "Cosmetic";
            break;
        case JsonInventory::LocalizedTagName::CRAFTSMANN_COLLECTION:
            j = "Craftsmann Collection";
            break;
        case JsonInventory::LocalizedTagName::CRAFT_ITEM:
            j = "Craft Item";
            break;
        case JsonInventory::LocalizedTagName::CRATE:
            j = "Crate";
            break;
        case JsonInventory::LocalizedTagName::CREEPY_CRAWLY_COLLECTION:
            j = "Creepy Crawly Collection";
            break;
        case JsonInventory::LocalizedTagName::CRIMSON_CACHE_COLLECTION:
            j = "Crimson Cache Collection";
            break;
        case JsonInventory::LocalizedTagName::DECORATED_WAR_HERO_COLLECTION:
            j = "Decorated War Hero Collection";
            break;
        case JsonInventory::LocalizedTagName::DECORATED_WEAPON:
            j = "Decorated Weapon";
            break;
        case JsonInventory::LocalizedTagName::DEMOMAN:
            j = "Demoman";
            break;
        case JsonInventory::LocalizedTagName::ELITE:
            j = "Elite";
            break;
        case JsonInventory::LocalizedTagName::EMPTY:
            j = "";
            break;
        case JsonInventory::LocalizedTagName::ENGINEER:
            j = "Engineer";
            break;
        case JsonInventory::LocalizedTagName::FACTORY_NEW:
            j = "Factory New";
            break;
        case JsonInventory::LocalizedTagName::FESTIVIZER:
            j = "Festivizer";
            break;
        case JsonInventory::LocalizedTagName::FIELD_TESTED:
            j = "Field-Tested";
            break;
        case JsonInventory::LocalizedTagName::FREELANCE:
            j = "Freelance";
            break;
        case JsonInventory::LocalizedTagName::GARGOYLE_COLLECTION:
            j = "Gargoyle Collection";
            break;
        case JsonInventory::LocalizedTagName::GENTLEMANNE_S_COLLECTION:
            j = "Gentlemanne's Collection";
            break;
        case JsonInventory::LocalizedTagName::GENUINE:
            j = "Genuine";
            break;
        case JsonInventory::LocalizedTagName::GIFT:
            j = "Gift";
            break;
        case JsonInventory::LocalizedTagName::GUN_METTLE_COSMETICS_COLLECTION:
            j = "Gun Mettle Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::HARVEST_COLLECTION:
            j = "Harvest Collection";
            break;
        case JsonInventory::LocalizedTagName::HAUNTED:
            j = "Haunted";
            break;
        case JsonInventory::LocalizedTagName::HEAVY:
            j = "Heavy";
            break;
        case JsonInventory::LocalizedTagName::INVASION_COLLECTIONS:
            j = "Invasion Collections";
            break;
        case JsonInventory::LocalizedTagName::JUNGLE_INFERNO_COLLECTIONS:
            j = "Jungle Inferno Collections";
            break;
        case JsonInventory::LocalizedTagName::JUNGLE_JACKPOT_COLLECTION:
            j = "Jungle Jackpot Collection";
            break;
        case JsonInventory::LocalizedTagName::KILLSTREAK_KIT:
            j = "Killstreak Kit";
            break;
        case JsonInventory::LocalizedTagName::MANN_CO_EVENTS_COLLECTION:
            j = "Mann Co. Events Collection";
            break;
        case JsonInventory::LocalizedTagName::MAYFLOWER_COSMETICS_COLLECTION:
            j = "Mayflower Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::MEDIC:
            j = "Medic";
            break;
        case JsonInventory::LocalizedTagName::MELEE_WEAPON:
            j = "Melee weapon";
            break;
        case JsonInventory::LocalizedTagName::MERCENARY:
            j = "Mercenary";
            break;
        case JsonInventory::LocalizedTagName::MINIMAL_WEAR:
            j = "Minimal Wear";
            break;
        case JsonInventory::LocalizedTagName::NORMAL:
            j = "Normal";
            break;
        case JsonInventory::LocalizedTagName::PACKAGE:
            j = "Package";
            break;
        case JsonInventory::LocalizedTagName::PARTY_FAVOR:
            j = "Party Favor";
            break;
        case JsonInventory::LocalizedTagName::POWERHOUSE_COLLECTION:
            j = "Powerhouse Collection";
            break;
        case JsonInventory::LocalizedTagName::PRIMARY_PDA:
            j = "Primary PDA";
            break;
        case JsonInventory::LocalizedTagName::PRIMARY_WEAPON:
            j = "Primary weapon";
            break;
        case JsonInventory::LocalizedTagName::PROFESSIONAL_KILLSTREAK_KIT:
            j = "Professional Killstreak Kit";
            break;
        case JsonInventory::LocalizedTagName::PYRO:
            j = "Pyro";
            break;
        case JsonInventory::LocalizedTagName::PYROLAND_COLLECTION:
            j = "Pyroland Collection";
            break;
        case JsonInventory::LocalizedTagName::QUARANTINED_COLLECTION_CASE:
            j = "Quarantined Collection Case";
            break;
        case JsonInventory::LocalizedTagName::RAINY_DAY_COSMETICS_COLLECTION:
            j = "Rainy Day Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::RECIPE:
            j = "Recipe";
            break;
        case JsonInventory::LocalizedTagName::SCOUT:
            j = "Scout";
            break;
        case JsonInventory::LocalizedTagName::SCREAM_FORTRESS_XIII_COLLECTION:
            j = "Scream Fortress XIII Collection";
            break;
        case JsonInventory::LocalizedTagName::SCREAM_FORTRESS_XII_COLLECTION:
            j = "Scream Fortress XII Collection";
            break;
        case JsonInventory::LocalizedTagName::SCREAM_FORTRESS_X_COLLECTION:
            j = "Scream Fortress X Collection";
            break;
        case JsonInventory::LocalizedTagName::SECONDARY_PDA:
            j = "Secondary PDA";
            break;
        case JsonInventory::LocalizedTagName::SECONDARY_WEAPON:
            j = "Secondary weapon";
            break;
        case JsonInventory::LocalizedTagName::SELF_MADE:
            j = "Self-Made";
            break;
        case JsonInventory::LocalizedTagName::SERVER_ENCHANTMENT:
            j = "Server Enchantment";
            break;
        case JsonInventory::LocalizedTagName::SNIPER:
            j = "Sniper";
            break;
        case JsonInventory::LocalizedTagName::SOLDIER:
            j = "Soldier";
            break;
        case JsonInventory::LocalizedTagName::SPELLBOOK_PAGE:
            j = "Spellbook Page";
            break;
        case JsonInventory::LocalizedTagName::SPOOKY_SPOILS_COLLECTION:
            j = "Spooky Spoils Collection";
            break;
        case JsonInventory::LocalizedTagName::SPY:
            j = "Spy";
            break;
        case JsonInventory::LocalizedTagName::STRANGE:
            j = "Strange";
            break;
        case JsonInventory::LocalizedTagName::STRANGE_PART:
            j = "Strange Part";
            break;
        case JsonInventory::LocalizedTagName::STRANGIFIER:
            j = "Strangifier";
            break;
        case JsonInventory::LocalizedTagName::SUMMER_2019_COSMETICS_COLLECTION:
            j = "Summer 2019 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::SUMMER_2020_COSMETICS_COLLECTION:
            j = "Summer 2020 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::SUMMER_2021_COSMETICS_COLLECTION:
            j = "Summer 2021 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::SUPPLY_CRATE:
            j = "Supply Crate";
            break;
        case JsonInventory::LocalizedTagName::TAUNT_1:
            j = "Taunt 1";
            break;
        case JsonInventory::LocalizedTagName::TEUFORT_COLLECTION:
            j = "Teufort Collection";
            break;
        case JsonInventory::LocalizedTagName::THE_GUN_METTLE_COLLECTIONS:
            j = "The Gun Mettle Collections";
            break;
        case JsonInventory::LocalizedTagName::THE_SCREAM_FORTRESS_COLLECTIONS:
            j = "The Scream Fortress Collections";
            break;
        case JsonInventory::LocalizedTagName::THE_TOUGH_BREAK_COLLECTIONS:
            j = "The Tough Break Collections";
            break;
        case JsonInventory::LocalizedTagName::TOOL:
            j = "Tool";
            break;
        case JsonInventory::LocalizedTagName::TOUGH_BREAK_COSMETICS_COLLECTION:
            j = "Tough Break Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::UNIQUE:
            j = "Unique";
            break;
        case JsonInventory::LocalizedTagName::UNLEASH_THE_BEAST_COSMETIC_COLLECTION:
            j = "Unleash the Beast Cosmetic Collection";
            break;
        case JsonInventory::LocalizedTagName::UNUSUAL:
            j = "Unusual";
            break;
        case JsonInventory::LocalizedTagName::UNUSUALIFIER:
            j = "Unusualifier";
            break;
        case JsonInventory::LocalizedTagName::USABLE_ITEM:
            j = "Usable Item";
            break;
        case JsonInventory::LocalizedTagName::VINTAGE:
            j = "Vintage";
            break;
        case JsonInventory::LocalizedTagName::VIOLET_VERMIN_COLLECTION:
            j = "Violet Vermin Collection";
            break;
        case JsonInventory::LocalizedTagName::WAR_PAINT:
            j = "War Paint";
            break;
        case JsonInventory::LocalizedTagName::WELL_WORN:
            j = "Well-Worn";
            break;
        case JsonInventory::LocalizedTagName::WICKED_WINDFALL_COLLECTION:
            j = "Wicked Windfall Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2016_COSMETICS_COLLECTION:
            j = "Winter 2016 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2017_COSMETICS_COLLECTION:
            j = "Winter 2017 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2018_COSMETICS_COLLECTION:
            j = "Winter 2018 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2019_COLLECTION:
            j = "Winter 2019 Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2019_COSMETICS_COLLECTION:
            j = "Winter 2019 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2020_COLLECTION:
            j = "Winter 2020 Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2020_COSMETICS_COLLECTION:
            j = "Winter 2020 Cosmetics Collection";
            break;
        case JsonInventory::LocalizedTagName::WINTER_2021_COSMETICS_COLLECTION:
            j = "Winter 2021 Cosmetics Collection";
            break;
        default:
            throw "This should not happen";
        }
    }
}
