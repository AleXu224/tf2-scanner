//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Schema data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

#include <unordered_map>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json & j, const std::shared_ptr<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json & j) {
            if (j.is_null()) return std::unique_ptr<T>(); else return std::unique_ptr<T>(new T(j.get<T>()));
        }
    };
}
#endif

namespace JsonSchema {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json & j, std::string property) {
        return get_optional<T>(j, property.data());
    }

    struct Attribute {
        std::string name;
        std::string attribute_class;
        double value;
    };

    enum class Craft : int { CRAFT_BAR, CRAFT_MATERIAL_BURNED, CRAFT_MATERIAL_VOODOOCURSED, CRAFT_TOKEN, EMPTY, HAT, HAUNTED_HAT, STRANGEPART, SUPPLY_CRATE, TOOL, WEAPON };

    enum class DropType : int { DROP, NONE };

    enum class HolidayRestriction : int { BIRTHDAY, CHRISTMAS, HALLOWEEN, HALLOWEEN_OR_FULLMOON };

    enum class ItemClass : int { BUNDLE, CLASS_TOKEN, CRAFT_ITEM, MAP_TOKEN, NO_ENTITY, SAXXY, SLOT_TOKEN, SUPPLY_CRATE, TF_POWERUP_BOTTLE, TF_WEAPON_BAT, TF_WEAPON_BAT_FISH, TF_WEAPON_BAT_GIFTWRAP, TF_WEAPON_BAT_WOOD, TF_WEAPON_BONESAW, TF_WEAPON_BOTTLE, TF_WEAPON_BREAKABLE_SIGN, TF_WEAPON_BUFF_ITEM, TF_WEAPON_BUILDER, TF_WEAPON_CANNON, TF_WEAPON_CHARGED_SMG, TF_WEAPON_CLEAVER, TF_WEAPON_CLUB, TF_WEAPON_COMPOUND_BOW, TF_WEAPON_CROSSBOW, TF_WEAPON_DRG_POMSON, TF_WEAPON_FIREAXE, TF_WEAPON_FISTS, TF_WEAPON_FLAMETHROWER, TF_WEAPON_FLAREGUN, TF_WEAPON_FLAREGUN_REVENGE, TF_WEAPON_GRAPPLINGHOOK, TF_WEAPON_GRENADELAUNCHER, TF_WEAPON_HANDGUN_SCOUT_PRIMARY, TF_WEAPON_HANDGUN_SCOUT_SECONDARY, TF_WEAPON_INVIS, TF_WEAPON_JAR, TF_WEAPON_JAR_GAS, TF_WEAPON_JAR_MILK, TF_WEAPON_KATANA, TF_WEAPON_KNIFE, TF_WEAPON_LASER_POINTER, TF_WEAPON_LUNCHBOX, TF_WEAPON_LUNCHBOX_DRINK, TF_WEAPON_MECHANICAL_ARM, TF_WEAPON_MEDIGUN, TF_WEAPON_MINIGUN, TF_WEAPON_PARACHUTE, TF_WEAPON_PARTICLE_CANNON, TF_WEAPON_PASSTIME_GUN, TF_WEAPON_PDA_ENGINEER_BUILD, TF_WEAPON_PDA_ENGINEER_DESTROY, TF_WEAPON_PDA_SPY, TF_WEAPON_PEP_BRAWLER_BLASTER, TF_WEAPON_PIPEBOMBLAUNCHER, TF_WEAPON_PISTOL, TF_WEAPON_RAYGUN, TF_WEAPON_REVOLVER, TF_WEAPON_ROBOT_ARM, TF_WEAPON_ROCKETLAUNCHER, TF_WEAPON_ROCKETLAUNCHER_AIRSTRIKE, TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT, TF_WEAPON_ROCKETLAUNCHER_FIREBALL, TF_WEAPON_ROCKETPACK, TF_WEAPON_SAPPER, TF_WEAPON_SCATTERGUN, TF_WEAPON_SENTRY_REVENGE, TF_WEAPON_SHOTGUN, TF_WEAPON_SHOTGUN_BUILDING_RESCUE, TF_WEAPON_SHOTGUN_PRIMARY, TF_WEAPON_SHOVEL, TF_WEAPON_SLAP, TF_WEAPON_SMG, TF_WEAPON_SNIPERRIFLE, TF_WEAPON_SNIPERRIFLE_CLASSIC, TF_WEAPON_SNIPERRIFLE_DECAP, TF_WEAPON_SODA_POPPER, TF_WEAPON_SPELLBOOK, TF_WEAPON_STICKBOMB, TF_WEAPON_SWORD, TF_WEAPON_SYRINGEGUN_MEDIC, TF_WEAPON_WRENCH, TF_WEARABLE, TF_WEARABLE_CAMPAIGN_ITEM, TF_WEARABLE_DEMOSHIELD, TF_WEARABLE_LEVELABLE_ITEM, TF_WEARABLE_RAZORBACK, TOOL, UPGRADE };

    enum class ItemSlot : int { ACTION, BUILDING, MELEE, MISC, PDA, PDA2, PRIMARY, SECONDARY, TAUNT, UTILITY };

   struct PerClassLoadoutSlots {
       ItemSlot soldier;
       std::shared_ptr<ItemSlot> heavy;
       std::shared_ptr<ItemSlot> pyro;
       std::shared_ptr<ItemSlot> engineer;
       std::shared_ptr<ItemSlot> demoman;
   };

   struct AdditionalHiddenBodygroups {
       std::shared_ptr<int64_t> hat;
       std::shared_ptr<int64_t> headphones;
       std::shared_ptr<int64_t> head;
       std::shared_ptr<int64_t> grenades;
   };

   struct Style {
       std::string name;
       std::shared_ptr<AdditionalHiddenBodygroups> additional_hidden_bodygroups;
   };

   struct Tool {
       std::string type;
       std::shared_ptr<std::map<std::string, bool>> usage_capabilities;
       std::shared_ptr<std::string> use_string;
       std::shared_ptr<std::string> restriction;
   };

    enum class UsedByClass : int { DEMOMAN, ENGINEER, HEAVY, MEDIC, PYRO, SCOUT, SNIPER, SOLDIER, SPY };

    struct Item {
        std::string name;
        int64_t defindex;
        ItemClass item_class;
        std::string item_type_name;
        std::string item_name;
        bool proper_name;
        // std::shared_ptr<ItemSlot> item_slot;
        // std::shared_ptr<std::string> model_player;
        // int64_t item_quality;
        // std::shared_ptr<std::string> image_inventory;
        // int64_t min_ilevel;
        // int64_t max_ilevel;
        // std::shared_ptr<std::string> image_url;
        // std::shared_ptr<std::string> image_url_large;
        // std::shared_ptr<Craft> craft_class;
        // std::shared_ptr<Craft> craft_material_type;
        // std::map<std::string, bool> capabilities;
        // std::shared_ptr<std::vector<UsedByClass>> used_by_classes;
        // std::shared_ptr<std::string> item_description;
//        std::shared_ptr<std::vector<Style>> styles;
        // std::shared_ptr<std::vector<Attribute>> attributes;
        // std::shared_ptr<DropType> drop_type;
        // std::shared_ptr<std::string> item_set;
        // std::shared_ptr<HolidayRestriction> holiday_restriction;
//        std::shared_ptr<PerClassLoadoutSlots> per_class_loadout_slots;
//        std::shared_ptr<Tool> tool;
    };

    struct Result {
        int64_t status;
        std::string items_game_url;
        std::vector<Item> items;
        std::shared_ptr<int64_t> next;
    };

    struct Schema {
        Result result;
    };
}

namespace nlohmann {
    void from_json(const json & j, JsonSchema::Attribute & x);
    void to_json(json & j, const JsonSchema::Attribute & x);

//    void from_json(const json & j, JsonSchema::PerClassLoadoutSlots & x);
//    void to_json(json & j, const JsonSchema::PerClassLoadoutSlots & x);

//    void from_json(const json & j, JsonSchema::AdditionalHiddenBodygroups & x);
//    void to_json(json & j, const JsonSchema::AdditionalHiddenBodygroups & x);

//    void from_json(const json & j, JsonSchema::Style & x);
//    void to_json(json & j, const JsonSchema::Style & x);

//    void from_json(const json & j, JsonSchema::Tool & x);
//    void to_json(json & j, const JsonSchema::Tool & x);

    void from_json(const json & j, JsonSchema::Item & x);
    void to_json(json & j, const JsonSchema::Item & x);

    void from_json(const json & j, JsonSchema::Result & x);
    void to_json(json & j, const JsonSchema::Result & x);

    void from_json(const json & j, JsonSchema::Schema & x);
    void to_json(json & j, const JsonSchema::Schema & x);

    void from_json(const json & j, JsonSchema::Craft & x);
    void to_json(json & j, const JsonSchema::Craft & x);

    void from_json(const json & j, JsonSchema::DropType & x);
    void to_json(json & j, const JsonSchema::DropType & x);

    void from_json(const json & j, JsonSchema::HolidayRestriction & x);
    void to_json(json & j, const JsonSchema::HolidayRestriction & x);

    void from_json(const json & j, JsonSchema::ItemClass & x);
    void to_json(json & j, const JsonSchema::ItemClass & x);

    void from_json(const json & j, JsonSchema::ItemSlot & x);
    void to_json(json & j, const JsonSchema::ItemSlot & x);

    void from_json(const json & j, JsonSchema::UsedByClass & x);
    void to_json(json & j, const JsonSchema::UsedByClass & x);

    inline void from_json(const json & j, JsonSchema::Attribute& x) {
        x.name = j.at("name").get<std::string>();
        x.attribute_class = j.at("class").get<std::string>();
        x.value = j.at("value").get<double>();
    }

    inline void to_json(json & j, const JsonSchema::Attribute & x) {
        j = json::object();
        j["name"] = x.name;
        j["class"] = x.attribute_class;
        j["value"] = x.value;
    }

//    inline void from_json(const json & j, JsonSchema::PerClassLoadoutSlots& x) {
//        x.soldier = j.at("Soldier").get<JsonSchema::ItemSlot>();
//        x.heavy = JsonSchema::get_optional<JsonSchema::ItemSlot>(j, "Heavy");
//        x.pyro = JsonSchema::get_optional<JsonSchema::ItemSlot>(j, "Pyro");
//        x.engineer = JsonSchema::get_optional<JsonSchema::ItemSlot>(j, "Engineer");
//        x.demoman = JsonSchema::get_optional<JsonSchema::ItemSlot>(j, "Demoman");
//    }

//    inline void to_json(json & j, const JsonSchema::PerClassLoadoutSlots & x) {
//        j = json::object();
//        j["Soldier"] = x.soldier;
//        j["Heavy"] = x.heavy;
//        j["Pyro"] = x.pyro;
//        j["Engineer"] = x.engineer;
//        j["Demoman"] = x.demoman;
//    }

//    inline void from_json(const json & j, JsonSchema::AdditionalHiddenBodygroups& x) {
//        x.hat = JsonSchema::get_optional<int64_t>(j, "hat");
//        x.headphones = JsonSchema::get_optional<int64_t>(j, "headphones");
//        x.head = JsonSchema::get_optional<int64_t>(j, "head");
//        x.grenades = JsonSchema::get_optional<int64_t>(j, "grenades");
//    }
//
//    inline void to_json(json & j, const JsonSchema::AdditionalHiddenBodygroups & x) {
//        j = json::object();
//        j["hat"] = x.hat;
//        j["headphones"] = x.headphones;
//        j["head"] = x.head;
//        j["grenades"] = x.grenades;
//    }

//    inline void from_json(const json & j, JsonSchema::Style& x) {
//        x.name = j.at("name").get<std::string>();
//        x.additional_hidden_bodygroups = JsonSchema::get_optional<JsonSchema::AdditionalHiddenBodygroups>(j, "additional_hidden_bodygroups");
//    }
//
//    inline void to_json(json & j, const JsonSchema::Style & x) {
//        j = json::object();
//        j["name"] = x.name;
//        j["additional_hidden_bodygroups"] = x.additional_hidden_bodygroups;
//    }

//    inline void from_json(const json & j, JsonSchema::Tool& x) {
//        x.type = j.at("type").get<std::string>();
//        x.usage_capabilities = JsonSchema::get_optional<std::map<std::string, bool>>(j, "usage_capabilities");
//        x.use_string = JsonSchema::get_optional<std::string>(j, "use_string");
//        x.restriction = JsonSchema::get_optional<std::string>(j, "restriction");
//    }
//
//    inline void to_json(json & j, const JsonSchema::Tool & x) {
//        j = json::object();
//        j["type"] = x.type;
//        j["usage_capabilities"] = x.usage_capabilities;
//        j["use_string"] = x.use_string;
//        j["restriction"] = x.restriction;
//    }

    inline void from_json(const json & j, JsonSchema::Item& x) {
        x.name = j.at("name").get<std::string>();
        x.defindex = j.at("defindex").get<int64_t>();
        x.item_class = j.at("item_class").get<JsonSchema::ItemClass>();
        x.item_type_name = j.at("item_type_name").get<std::string>();
        x.item_name = j.at("item_name").get<std::string>();
        x.proper_name = j.at("proper_name").get<bool>();
        // x.item_slot = JsonSchema::get_optional<JsonSchema::ItemSlot>(j, "item_slot");
        // x.model_player = JsonSchema::get_optional<std::string>(j, "model_player");
        // x.item_quality = j.at("item_quality").get<int64_t>();
        // x.image_inventory = JsonSchema::get_optional<std::string>(j, "image_inventory");
        // x.min_ilevel = j.at("min_ilevel").get<int64_t>();
        // x.max_ilevel = j.at("max_ilevel").get<int64_t>();
        // x.image_url = JsonSchema::get_optional<std::string>(j, "image_url");
        // x.image_url_large = JsonSchema::get_optional<std::string>(j, "image_url_large");
        // x.craft_class = JsonSchema::get_optional<JsonSchema::Craft>(j, "craft_class");
        // x.craft_material_type = JsonSchema::get_optional<JsonSchema::Craft>(j, "craft_material_type");
        // x.capabilities = j.at("capabilities").get<std::map<std::string, bool>>();
        // x.used_by_classes = JsonSchema::get_optional<std::vector<JsonSchema::UsedByClass>>(j, "used_by_classes");
        // x.item_description = JsonSchema::get_optional<std::string>(j, "item_description");
//        x.styles = JsonSchema::get_optional<std::vector<JsonSchema::Style>>(j, "styles");
        // x.attributes = JsonSchema::get_optional<std::vector<JsonSchema::Attribute>>(j, "attributes");
        // x.drop_type = JsonSchema::get_optional<JsonSchema::DropType>(j, "drop_type");
        // x.item_set = JsonSchema::get_optional<std::string>(j, "item_set");
        // x.holiday_restriction = JsonSchema::get_optional<JsonSchema::HolidayRestriction>(j, "holiday_restriction");
//        x.per_class_loadout_slots = JsonSchema::get_optional<JsonSchema::PerClassLoadoutSlots>(j, "per_class_loadout_slots");
//        x.tool = JsonSchema::get_optional<JsonSchema::Tool>(j, "tool");
    }

    inline void to_json(json & j, const JsonSchema::Item & x) {
        j = json::object();
        j["name"] = x.name;
        j["defindex"] = x.defindex;
        j["item_class"] = x.item_class;
        j["item_type_name"] = x.item_type_name;
        j["item_name"] = x.item_name;
        j["proper_name"] = x.proper_name;
        // j["item_slot"] = x.item_slot;
        // j["model_player"] = x.model_player;
        // j["item_quality"] = x.item_quality;
        // j["image_inventory"] = x.image_inventory;
        // j["min_ilevel"] = x.min_ilevel;
        // j["max_ilevel"] = x.max_ilevel;
        // j["image_url"] = x.image_url;
        // j["image_url_large"] = x.image_url_large;
        // j["craft_class"] = x.craft_class;
        // j["craft_material_type"] = x.craft_material_type;
        // j["capabilities"] = x.capabilities;
        // j["used_by_classes"] = x.used_by_classes;
        // j["item_description"] = x.item_description;
//        j["styles"] = x.styles;
        // j["attributes"] = x.attributes;
        // j["drop_type"] = x.drop_type;
        // j["item_set"] = x.item_set;
        // j["holiday_restriction"] = x.holiday_restriction;
//        j["per_class_loadout_slots"] = x.per_class_loadout_slots;
//        j["tool"] = x.tool;
    }

    inline void from_json(const json & j, JsonSchema::Result& x) {
        x.status = j.at("status").get<int64_t>();
        x.items_game_url = j.at("items_game_url").get<std::string>();
        x.items = j.at("items").get<std::vector<JsonSchema::Item>>();
        x.next = JsonSchema::get_optional<int64_t>(j, "next");
    }

    inline void to_json(json & j, const JsonSchema::Result & x) {
        j = json::object();
        j["status"] = x.status;
        j["items_game_url"] = x.items_game_url;
        j["items"] = x.items;
        j["next"] = x.next;
    }

    inline void from_json(const json & j, JsonSchema::Schema& x) {
        x.result = j.at("result").get<JsonSchema::Result>();
    }

    inline void to_json(json & j, const JsonSchema::Schema & x) {
        j = json::object();
        j["result"] = x.result;
    }

    inline void from_json(const json & j, JsonSchema::Craft & x) {
        if (j == "craft_bar") x = JsonSchema::Craft::CRAFT_BAR;
        else if (j == "craft_material_burned") x = JsonSchema::Craft::CRAFT_MATERIAL_BURNED;
        else if (j == "craft_material_voodoocursed") x = JsonSchema::Craft::CRAFT_MATERIAL_VOODOOCURSED;
        else if (j == "craft_token") x = JsonSchema::Craft::CRAFT_TOKEN;
        else if (j == "") x = JsonSchema::Craft::EMPTY;
        else if (j == "hat") x = JsonSchema::Craft::HAT;
        else if (j == "haunted_hat") x = JsonSchema::Craft::HAUNTED_HAT;
        else if (j == "strangepart") x = JsonSchema::Craft::STRANGEPART;
        else if (j == "supply_crate") x = JsonSchema::Craft::SUPPLY_CRATE;
        else if (j == "tool") x = JsonSchema::Craft::TOOL;
        else if (j == "weapon") x = JsonSchema::Craft::WEAPON;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const JsonSchema::Craft & x) {
        switch (x) {
            case JsonSchema::Craft::CRAFT_BAR: j = "craft_bar"; break;
            case JsonSchema::Craft::CRAFT_MATERIAL_BURNED: j = "craft_material_burned"; break;
            case JsonSchema::Craft::CRAFT_MATERIAL_VOODOOCURSED: j = "craft_material_voodoocursed"; break;
            case JsonSchema::Craft::CRAFT_TOKEN: j = "craft_token"; break;
            case JsonSchema::Craft::EMPTY: j = ""; break;
            case JsonSchema::Craft::HAT: j = "hat"; break;
            case JsonSchema::Craft::HAUNTED_HAT: j = "haunted_hat"; break;
            case JsonSchema::Craft::STRANGEPART: j = "strangepart"; break;
            case JsonSchema::Craft::SUPPLY_CRATE: j = "supply_crate"; break;
            case JsonSchema::Craft::TOOL: j = "tool"; break;
            case JsonSchema::Craft::WEAPON: j = "weapon"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, JsonSchema::DropType & x) {
        if (j == "drop") x = JsonSchema::DropType::DROP;
        else if (j == "none") x = JsonSchema::DropType::NONE;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const JsonSchema::DropType & x) {
        switch (x) {
            case JsonSchema::DropType::DROP: j = "drop"; break;
            case JsonSchema::DropType::NONE: j = "none"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, JsonSchema::HolidayRestriction & x) {
        if (j == "birthday") x = JsonSchema::HolidayRestriction::BIRTHDAY;
        else if (j == "christmas") x = JsonSchema::HolidayRestriction::CHRISTMAS;
        else if (j == "halloween") x = JsonSchema::HolidayRestriction::HALLOWEEN;
        else if (j == "halloween_or_fullmoon") x = JsonSchema::HolidayRestriction::HALLOWEEN_OR_FULLMOON;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const JsonSchema::HolidayRestriction & x) {
        switch (x) {
            case JsonSchema::HolidayRestriction::BIRTHDAY: j = "birthday"; break;
            case JsonSchema::HolidayRestriction::CHRISTMAS: j = "christmas"; break;
            case JsonSchema::HolidayRestriction::HALLOWEEN: j = "halloween"; break;
            case JsonSchema::HolidayRestriction::HALLOWEEN_OR_FULLMOON: j = "halloween_or_fullmoon"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, JsonSchema::ItemClass & x) {
        static std::unordered_map<std::string, JsonSchema::ItemClass> enumValues {
            {"bundle", JsonSchema::ItemClass::BUNDLE},
            {"class_token", JsonSchema::ItemClass::CLASS_TOKEN},
            {"craft_item", JsonSchema::ItemClass::CRAFT_ITEM},
            {"map_token", JsonSchema::ItemClass::MAP_TOKEN},
            {"no_entity", JsonSchema::ItemClass::NO_ENTITY},
            {"saxxy", JsonSchema::ItemClass::SAXXY},
            {"slot_token", JsonSchema::ItemClass::SLOT_TOKEN},
            {"supply_crate", JsonSchema::ItemClass::SUPPLY_CRATE},
            {"tf_powerup_bottle", JsonSchema::ItemClass::TF_POWERUP_BOTTLE},
            {"tf_weapon_bat", JsonSchema::ItemClass::TF_WEAPON_BAT},
            {"tf_weapon_bat_fish", JsonSchema::ItemClass::TF_WEAPON_BAT_FISH},
            {"tf_weapon_bat_giftwrap", JsonSchema::ItemClass::TF_WEAPON_BAT_GIFTWRAP},
            {"tf_weapon_bat_wood", JsonSchema::ItemClass::TF_WEAPON_BAT_WOOD},
            {"tf_weapon_bonesaw", JsonSchema::ItemClass::TF_WEAPON_BONESAW},
            {"tf_weapon_bottle", JsonSchema::ItemClass::TF_WEAPON_BOTTLE},
            {"tf_weapon_breakable_sign", JsonSchema::ItemClass::TF_WEAPON_BREAKABLE_SIGN},
            {"tf_weapon_buff_item", JsonSchema::ItemClass::TF_WEAPON_BUFF_ITEM},
            {"tf_weapon_builder", JsonSchema::ItemClass::TF_WEAPON_BUILDER},
            {"tf_weapon_cannon", JsonSchema::ItemClass::TF_WEAPON_CANNON},
            {"tf_weapon_charged_smg", JsonSchema::ItemClass::TF_WEAPON_CHARGED_SMG},
            {"tf_weapon_cleaver", JsonSchema::ItemClass::TF_WEAPON_CLEAVER},
            {"tf_weapon_club", JsonSchema::ItemClass::TF_WEAPON_CLUB},
            {"tf_weapon_compound_bow", JsonSchema::ItemClass::TF_WEAPON_COMPOUND_BOW},
            {"tf_weapon_crossbow", JsonSchema::ItemClass::TF_WEAPON_CROSSBOW},
            {"tf_weapon_drg_pomson", JsonSchema::ItemClass::TF_WEAPON_DRG_POMSON},
            {"tf_weapon_fireaxe", JsonSchema::ItemClass::TF_WEAPON_FIREAXE},
            {"tf_weapon_fists", JsonSchema::ItemClass::TF_WEAPON_FISTS},
            {"tf_weapon_flamethrower", JsonSchema::ItemClass::TF_WEAPON_FLAMETHROWER},
            {"tf_weapon_flaregun", JsonSchema::ItemClass::TF_WEAPON_FLAREGUN},
            {"tf_weapon_flaregun_revenge", JsonSchema::ItemClass::TF_WEAPON_FLAREGUN_REVENGE},
            {"tf_weapon_grapplinghook", JsonSchema::ItemClass::TF_WEAPON_GRAPPLINGHOOK},
            {"tf_weapon_grenadelauncher", JsonSchema::ItemClass::TF_WEAPON_GRENADELAUNCHER},
            {"tf_weapon_handgun_scout_primary", JsonSchema::ItemClass::TF_WEAPON_HANDGUN_SCOUT_PRIMARY},
            {"tf_weapon_handgun_scout_secondary", JsonSchema::ItemClass::TF_WEAPON_HANDGUN_SCOUT_SECONDARY},
            {"tf_weapon_invis", JsonSchema::ItemClass::TF_WEAPON_INVIS},
            {"tf_weapon_jar", JsonSchema::ItemClass::TF_WEAPON_JAR},
            {"tf_weapon_jar_gas", JsonSchema::ItemClass::TF_WEAPON_JAR_GAS},
            {"tf_weapon_jar_milk", JsonSchema::ItemClass::TF_WEAPON_JAR_MILK},
            {"tf_weapon_katana", JsonSchema::ItemClass::TF_WEAPON_KATANA},
            {"tf_weapon_knife", JsonSchema::ItemClass::TF_WEAPON_KNIFE},
            {"tf_weapon_laser_pointer", JsonSchema::ItemClass::TF_WEAPON_LASER_POINTER},
            {"tf_weapon_lunchbox", JsonSchema::ItemClass::TF_WEAPON_LUNCHBOX},
            {"tf_weapon_lunchbox_drink", JsonSchema::ItemClass::TF_WEAPON_LUNCHBOX_DRINK},
            {"tf_weapon_mechanical_arm", JsonSchema::ItemClass::TF_WEAPON_MECHANICAL_ARM},
            {"tf_weapon_medigun", JsonSchema::ItemClass::TF_WEAPON_MEDIGUN},
            {"tf_weapon_minigun", JsonSchema::ItemClass::TF_WEAPON_MINIGUN},
            {"tf_weapon_parachute", JsonSchema::ItemClass::TF_WEAPON_PARACHUTE},
            {"tf_weapon_particle_cannon", JsonSchema::ItemClass::TF_WEAPON_PARTICLE_CANNON},
            {"tf_weapon_passtime_gun", JsonSchema::ItemClass::TF_WEAPON_PASSTIME_GUN},
            {"tf_weapon_pda_engineer_build", JsonSchema::ItemClass::TF_WEAPON_PDA_ENGINEER_BUILD},
            {"tf_weapon_pda_engineer_destroy", JsonSchema::ItemClass::TF_WEAPON_PDA_ENGINEER_DESTROY},
            {"tf_weapon_pda_spy", JsonSchema::ItemClass::TF_WEAPON_PDA_SPY},
            {"tf_weapon_pep_brawler_blaster", JsonSchema::ItemClass::TF_WEAPON_PEP_BRAWLER_BLASTER},
            {"tf_weapon_pipebomblauncher", JsonSchema::ItemClass::TF_WEAPON_PIPEBOMBLAUNCHER},
            {"tf_weapon_pistol", JsonSchema::ItemClass::TF_WEAPON_PISTOL},
            {"tf_weapon_raygun", JsonSchema::ItemClass::TF_WEAPON_RAYGUN},
            {"tf_weapon_revolver", JsonSchema::ItemClass::TF_WEAPON_REVOLVER},
            {"tf_weapon_robot_arm", JsonSchema::ItemClass::TF_WEAPON_ROBOT_ARM},
            {"tf_weapon_rocketlauncher", JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER},
            {"tf_weapon_rocketlauncher_airstrike", JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER_AIRSTRIKE},
            {"tf_weapon_rocketlauncher_directhit", JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT},
            {"tf_weapon_rocketlauncher_fireball", JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER_FIREBALL},
            {"tf_weapon_rocketpack", JsonSchema::ItemClass::TF_WEAPON_ROCKETPACK},
            {"tf_weapon_sapper", JsonSchema::ItemClass::TF_WEAPON_SAPPER},
            {"tf_weapon_scattergun", JsonSchema::ItemClass::TF_WEAPON_SCATTERGUN},
            {"tf_weapon_sentry_revenge", JsonSchema::ItemClass::TF_WEAPON_SENTRY_REVENGE},
            {"tf_weapon_shotgun", JsonSchema::ItemClass::TF_WEAPON_SHOTGUN},
            {"tf_weapon_shotgun_building_rescue", JsonSchema::ItemClass::TF_WEAPON_SHOTGUN_BUILDING_RESCUE},
            {"tf_weapon_shotgun_primary", JsonSchema::ItemClass::TF_WEAPON_SHOTGUN_PRIMARY},
            {"tf_weapon_shovel", JsonSchema::ItemClass::TF_WEAPON_SHOVEL},
            {"tf_weapon_slap", JsonSchema::ItemClass::TF_WEAPON_SLAP},
            {"tf_weapon_smg", JsonSchema::ItemClass::TF_WEAPON_SMG},
            {"tf_weapon_sniperrifle", JsonSchema::ItemClass::TF_WEAPON_SNIPERRIFLE},
            {"tf_weapon_sniperrifle_classic", JsonSchema::ItemClass::TF_WEAPON_SNIPERRIFLE_CLASSIC},
            {"tf_weapon_sniperrifle_decap", JsonSchema::ItemClass::TF_WEAPON_SNIPERRIFLE_DECAP},
            {"tf_weapon_soda_popper", JsonSchema::ItemClass::TF_WEAPON_SODA_POPPER},
            {"tf_weapon_spellbook", JsonSchema::ItemClass::TF_WEAPON_SPELLBOOK},
            {"tf_weapon_stickbomb", JsonSchema::ItemClass::TF_WEAPON_STICKBOMB},
            {"tf_weapon_sword", JsonSchema::ItemClass::TF_WEAPON_SWORD},
            {"tf_weapon_syringegun_medic", JsonSchema::ItemClass::TF_WEAPON_SYRINGEGUN_MEDIC},
            {"tf_weapon_wrench", JsonSchema::ItemClass::TF_WEAPON_WRENCH},
            {"tf_wearable", JsonSchema::ItemClass::TF_WEARABLE},
            {"tf_wearable_campaign_item", JsonSchema::ItemClass::TF_WEARABLE_CAMPAIGN_ITEM},
            {"tf_wearable_demoshield", JsonSchema::ItemClass::TF_WEARABLE_DEMOSHIELD},
            {"tf_wearable_levelable_item", JsonSchema::ItemClass::TF_WEARABLE_LEVELABLE_ITEM},
            {"tf_wearable_razorback", JsonSchema::ItemClass::TF_WEARABLE_RAZORBACK},
            {"tool", JsonSchema::ItemClass::TOOL},
            {"upgrade", JsonSchema::ItemClass::UPGRADE},
        };
        auto iter = enumValues.find(j.get<std::string>());
        if (iter != enumValues.end()) {
            x = iter->second;
        }
    }

    inline void to_json(json & j, const JsonSchema::ItemClass & x) {
        switch (x) {
            case JsonSchema::ItemClass::BUNDLE: j = "bundle"; break;
            case JsonSchema::ItemClass::CLASS_TOKEN: j = "class_token"; break;
            case JsonSchema::ItemClass::CRAFT_ITEM: j = "craft_item"; break;
            case JsonSchema::ItemClass::MAP_TOKEN: j = "map_token"; break;
            case JsonSchema::ItemClass::NO_ENTITY: j = "no_entity"; break;
            case JsonSchema::ItemClass::SAXXY: j = "saxxy"; break;
            case JsonSchema::ItemClass::SLOT_TOKEN: j = "slot_token"; break;
            case JsonSchema::ItemClass::SUPPLY_CRATE: j = "supply_crate"; break;
            case JsonSchema::ItemClass::TF_POWERUP_BOTTLE: j = "tf_powerup_bottle"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BAT: j = "tf_weapon_bat"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BAT_FISH: j = "tf_weapon_bat_fish"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BAT_GIFTWRAP: j = "tf_weapon_bat_giftwrap"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BAT_WOOD: j = "tf_weapon_bat_wood"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BONESAW: j = "tf_weapon_bonesaw"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BOTTLE: j = "tf_weapon_bottle"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BREAKABLE_SIGN: j = "tf_weapon_breakable_sign"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BUFF_ITEM: j = "tf_weapon_buff_item"; break;
            case JsonSchema::ItemClass::TF_WEAPON_BUILDER: j = "tf_weapon_builder"; break;
            case JsonSchema::ItemClass::TF_WEAPON_CANNON: j = "tf_weapon_cannon"; break;
            case JsonSchema::ItemClass::TF_WEAPON_CHARGED_SMG: j = "tf_weapon_charged_smg"; break;
            case JsonSchema::ItemClass::TF_WEAPON_CLEAVER: j = "tf_weapon_cleaver"; break;
            case JsonSchema::ItemClass::TF_WEAPON_CLUB: j = "tf_weapon_club"; break;
            case JsonSchema::ItemClass::TF_WEAPON_COMPOUND_BOW: j = "tf_weapon_compound_bow"; break;
            case JsonSchema::ItemClass::TF_WEAPON_CROSSBOW: j = "tf_weapon_crossbow"; break;
            case JsonSchema::ItemClass::TF_WEAPON_DRG_POMSON: j = "tf_weapon_drg_pomson"; break;
            case JsonSchema::ItemClass::TF_WEAPON_FIREAXE: j = "tf_weapon_fireaxe"; break;
            case JsonSchema::ItemClass::TF_WEAPON_FISTS: j = "tf_weapon_fists"; break;
            case JsonSchema::ItemClass::TF_WEAPON_FLAMETHROWER: j = "tf_weapon_flamethrower"; break;
            case JsonSchema::ItemClass::TF_WEAPON_FLAREGUN: j = "tf_weapon_flaregun"; break;
            case JsonSchema::ItemClass::TF_WEAPON_FLAREGUN_REVENGE: j = "tf_weapon_flaregun_revenge"; break;
            case JsonSchema::ItemClass::TF_WEAPON_GRAPPLINGHOOK: j = "tf_weapon_grapplinghook"; break;
            case JsonSchema::ItemClass::TF_WEAPON_GRENADELAUNCHER: j = "tf_weapon_grenadelauncher"; break;
            case JsonSchema::ItemClass::TF_WEAPON_HANDGUN_SCOUT_PRIMARY: j = "tf_weapon_handgun_scout_primary"; break;
            case JsonSchema::ItemClass::TF_WEAPON_HANDGUN_SCOUT_SECONDARY: j = "tf_weapon_handgun_scout_secondary"; break;
            case JsonSchema::ItemClass::TF_WEAPON_INVIS: j = "tf_weapon_invis"; break;
            case JsonSchema::ItemClass::TF_WEAPON_JAR: j = "tf_weapon_jar"; break;
            case JsonSchema::ItemClass::TF_WEAPON_JAR_GAS: j = "tf_weapon_jar_gas"; break;
            case JsonSchema::ItemClass::TF_WEAPON_JAR_MILK: j = "tf_weapon_jar_milk"; break;
            case JsonSchema::ItemClass::TF_WEAPON_KATANA: j = "tf_weapon_katana"; break;
            case JsonSchema::ItemClass::TF_WEAPON_KNIFE: j = "tf_weapon_knife"; break;
            case JsonSchema::ItemClass::TF_WEAPON_LASER_POINTER: j = "tf_weapon_laser_pointer"; break;
            case JsonSchema::ItemClass::TF_WEAPON_LUNCHBOX: j = "tf_weapon_lunchbox"; break;
            case JsonSchema::ItemClass::TF_WEAPON_LUNCHBOX_DRINK: j = "tf_weapon_lunchbox_drink"; break;
            case JsonSchema::ItemClass::TF_WEAPON_MECHANICAL_ARM: j = "tf_weapon_mechanical_arm"; break;
            case JsonSchema::ItemClass::TF_WEAPON_MEDIGUN: j = "tf_weapon_medigun"; break;
            case JsonSchema::ItemClass::TF_WEAPON_MINIGUN: j = "tf_weapon_minigun"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PARACHUTE: j = "tf_weapon_parachute"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PARTICLE_CANNON: j = "tf_weapon_particle_cannon"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PASSTIME_GUN: j = "tf_weapon_passtime_gun"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PDA_ENGINEER_BUILD: j = "tf_weapon_pda_engineer_build"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PDA_ENGINEER_DESTROY: j = "tf_weapon_pda_engineer_destroy"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PDA_SPY: j = "tf_weapon_pda_spy"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PEP_BRAWLER_BLASTER: j = "tf_weapon_pep_brawler_blaster"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PIPEBOMBLAUNCHER: j = "tf_weapon_pipebomblauncher"; break;
            case JsonSchema::ItemClass::TF_WEAPON_PISTOL: j = "tf_weapon_pistol"; break;
            case JsonSchema::ItemClass::TF_WEAPON_RAYGUN: j = "tf_weapon_raygun"; break;
            case JsonSchema::ItemClass::TF_WEAPON_REVOLVER: j = "tf_weapon_revolver"; break;
            case JsonSchema::ItemClass::TF_WEAPON_ROBOT_ARM: j = "tf_weapon_robot_arm"; break;
            case JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER: j = "tf_weapon_rocketlauncher"; break;
            case JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER_AIRSTRIKE: j = "tf_weapon_rocketlauncher_airstrike"; break;
            case JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT: j = "tf_weapon_rocketlauncher_directhit"; break;
            case JsonSchema::ItemClass::TF_WEAPON_ROCKETLAUNCHER_FIREBALL: j = "tf_weapon_rocketlauncher_fireball"; break;
            case JsonSchema::ItemClass::TF_WEAPON_ROCKETPACK: j = "tf_weapon_rocketpack"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SAPPER: j = "tf_weapon_sapper"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SCATTERGUN: j = "tf_weapon_scattergun"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SENTRY_REVENGE: j = "tf_weapon_sentry_revenge"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SHOTGUN: j = "tf_weapon_shotgun"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SHOTGUN_BUILDING_RESCUE: j = "tf_weapon_shotgun_building_rescue"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SHOTGUN_PRIMARY: j = "tf_weapon_shotgun_primary"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SHOVEL: j = "tf_weapon_shovel"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SLAP: j = "tf_weapon_slap"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SMG: j = "tf_weapon_smg"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SNIPERRIFLE: j = "tf_weapon_sniperrifle"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SNIPERRIFLE_CLASSIC: j = "tf_weapon_sniperrifle_classic"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SNIPERRIFLE_DECAP: j = "tf_weapon_sniperrifle_decap"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SODA_POPPER: j = "tf_weapon_soda_popper"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SPELLBOOK: j = "tf_weapon_spellbook"; break;
            case JsonSchema::ItemClass::TF_WEAPON_STICKBOMB: j = "tf_weapon_stickbomb"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SWORD: j = "tf_weapon_sword"; break;
            case JsonSchema::ItemClass::TF_WEAPON_SYRINGEGUN_MEDIC: j = "tf_weapon_syringegun_medic"; break;
            case JsonSchema::ItemClass::TF_WEAPON_WRENCH: j = "tf_weapon_wrench"; break;
            case JsonSchema::ItemClass::TF_WEARABLE: j = "tf_wearable"; break;
            case JsonSchema::ItemClass::TF_WEARABLE_CAMPAIGN_ITEM: j = "tf_wearable_campaign_item"; break;
            case JsonSchema::ItemClass::TF_WEARABLE_DEMOSHIELD: j = "tf_wearable_demoshield"; break;
            case JsonSchema::ItemClass::TF_WEARABLE_LEVELABLE_ITEM: j = "tf_wearable_levelable_item"; break;
            case JsonSchema::ItemClass::TF_WEARABLE_RAZORBACK: j = "tf_wearable_razorback"; break;
            case JsonSchema::ItemClass::TOOL: j = "tool"; break;
            case JsonSchema::ItemClass::UPGRADE: j = "upgrade"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, JsonSchema::ItemSlot & x) {
        if (j == "action") x = JsonSchema::ItemSlot::ACTION;
        else if (j == "building") x = JsonSchema::ItemSlot::BUILDING;
        else if (j == "melee") x = JsonSchema::ItemSlot::MELEE;
        else if (j == "misc") x = JsonSchema::ItemSlot::MISC;
        else if (j == "pda") x = JsonSchema::ItemSlot::PDA;
        else if (j == "pda2") x = JsonSchema::ItemSlot::PDA2;
        else if (j == "primary") x = JsonSchema::ItemSlot::PRIMARY;
        else if (j == "secondary") x = JsonSchema::ItemSlot::SECONDARY;
        else if (j == "taunt") x = JsonSchema::ItemSlot::TAUNT;
        else if (j == "utility") x = JsonSchema::ItemSlot::UTILITY;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const JsonSchema::ItemSlot & x) {
        switch (x) {
            case JsonSchema::ItemSlot::ACTION: j = "action"; break;
            case JsonSchema::ItemSlot::BUILDING: j = "building"; break;
            case JsonSchema::ItemSlot::MELEE: j = "melee"; break;
            case JsonSchema::ItemSlot::MISC: j = "misc"; break;
            case JsonSchema::ItemSlot::PDA: j = "pda"; break;
            case JsonSchema::ItemSlot::PDA2: j = "pda2"; break;
            case JsonSchema::ItemSlot::PRIMARY: j = "primary"; break;
            case JsonSchema::ItemSlot::SECONDARY: j = "secondary"; break;
            case JsonSchema::ItemSlot::TAUNT: j = "taunt"; break;
            case JsonSchema::ItemSlot::UTILITY: j = "utility"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, JsonSchema::UsedByClass & x) {
        if (j == "Demoman") x = JsonSchema::UsedByClass::DEMOMAN;
        else if (j == "Engineer") x = JsonSchema::UsedByClass::ENGINEER;
        else if (j == "Heavy") x = JsonSchema::UsedByClass::HEAVY;
        else if (j == "Medic") x = JsonSchema::UsedByClass::MEDIC;
        else if (j == "Pyro") x = JsonSchema::UsedByClass::PYRO;
        else if (j == "Scout") x = JsonSchema::UsedByClass::SCOUT;
        else if (j == "Sniper") x = JsonSchema::UsedByClass::SNIPER;
        else if (j == "Soldier") x = JsonSchema::UsedByClass::SOLDIER;
        else if (j == "Spy") x = JsonSchema::UsedByClass::SPY;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const JsonSchema::UsedByClass & x) {
        switch (x) {
            case JsonSchema::UsedByClass::DEMOMAN: j = "Demoman"; break;
            case JsonSchema::UsedByClass::ENGINEER: j = "Engineer"; break;
            case JsonSchema::UsedByClass::HEAVY: j = "Heavy"; break;
            case JsonSchema::UsedByClass::MEDIC: j = "Medic"; break;
            case JsonSchema::UsedByClass::PYRO: j = "Pyro"; break;
            case JsonSchema::UsedByClass::SCOUT: j = "Scout"; break;
            case JsonSchema::UsedByClass::SNIPER: j = "Sniper"; break;
            case JsonSchema::UsedByClass::SOLDIER: j = "Soldier"; break;
            case JsonSchema::UsedByClass::SPY: j = "Spy"; break;
            default: throw "This should not happen";
        }
    }
}
