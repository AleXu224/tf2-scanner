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

namespace quicktype {
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
        std::shared_ptr<ItemSlot> item_slot;
        std::shared_ptr<std::string> model_player;
        int64_t item_quality;
        std::shared_ptr<std::string> image_inventory;
        int64_t min_ilevel;
        int64_t max_ilevel;
        std::shared_ptr<std::string> image_url;
        std::shared_ptr<std::string> image_url_large;
        std::shared_ptr<Craft> craft_class;
        std::shared_ptr<Craft> craft_material_type;
        std::map<std::string, bool> capabilities;
        std::shared_ptr<std::vector<UsedByClass>> used_by_classes;
        std::shared_ptr<std::string> item_description;
        std::shared_ptr<std::vector<Style>> styles;
        std::shared_ptr<std::vector<Attribute>> attributes;
        std::shared_ptr<DropType> drop_type;
        std::shared_ptr<std::string> item_set;
        std::shared_ptr<HolidayRestriction> holiday_restriction;
        std::shared_ptr<PerClassLoadoutSlots> per_class_loadout_slots;
        std::shared_ptr<Tool> tool;
    };

    struct Result {
        int64_t status;
        std::string items_game_url;
        std::vector<Item> items;
        std::shared_ptr<int64_t> next;
    };

    struct SchemaElement {
        Result result;
    };

    using Schema = std::vector<SchemaElement>;
}

namespace quicktype {
    using Schema = std::vector<SchemaElement>;
}

namespace nlohmann {
    void from_json(const json & j, quicktype::Attribute & x);
    void to_json(json & j, const quicktype::Attribute & x);

    void from_json(const json & j, quicktype::PerClassLoadoutSlots & x);
    void to_json(json & j, const quicktype::PerClassLoadoutSlots & x);

    void from_json(const json & j, quicktype::AdditionalHiddenBodygroups & x);
    void to_json(json & j, const quicktype::AdditionalHiddenBodygroups & x);

    void from_json(const json & j, quicktype::Style & x);
    void to_json(json & j, const quicktype::Style & x);

    void from_json(const json & j, quicktype::Tool & x);
    void to_json(json & j, const quicktype::Tool & x);

    void from_json(const json & j, quicktype::Item & x);
    void to_json(json & j, const quicktype::Item & x);

    void from_json(const json & j, quicktype::Result & x);
    void to_json(json & j, const quicktype::Result & x);

    void from_json(const json & j, quicktype::SchemaElement & x);
    void to_json(json & j, const quicktype::SchemaElement & x);

    void from_json(const json & j, quicktype::Craft & x);
    void to_json(json & j, const quicktype::Craft & x);

    void from_json(const json & j, quicktype::DropType & x);
    void to_json(json & j, const quicktype::DropType & x);

    void from_json(const json & j, quicktype::HolidayRestriction & x);
    void to_json(json & j, const quicktype::HolidayRestriction & x);

    void from_json(const json & j, quicktype::ItemClass & x);
    void to_json(json & j, const quicktype::ItemClass & x);

    void from_json(const json & j, quicktype::ItemSlot & x);
    void to_json(json & j, const quicktype::ItemSlot & x);

    void from_json(const json & j, quicktype::UsedByClass & x);
    void to_json(json & j, const quicktype::UsedByClass & x);

    inline void from_json(const json & j, quicktype::Attribute& x) {
        x.name = j.at("name").get<std::string>();
        x.attribute_class = j.at("class").get<std::string>();
        x.value = j.at("value").get<double>();
    }

    inline void to_json(json & j, const quicktype::Attribute & x) {
        j = json::object();
        j["name"] = x.name;
        j["class"] = x.attribute_class;
        j["value"] = x.value;
    }

    inline void from_json(const json & j, quicktype::PerClassLoadoutSlots& x) {
        x.soldier = j.at("Soldier").get<quicktype::ItemSlot>();
        x.heavy = quicktype::get_optional<quicktype::ItemSlot>(j, "Heavy");
        x.pyro = quicktype::get_optional<quicktype::ItemSlot>(j, "Pyro");
        x.engineer = quicktype::get_optional<quicktype::ItemSlot>(j, "Engineer");
        x.demoman = quicktype::get_optional<quicktype::ItemSlot>(j, "Demoman");
    }

    inline void to_json(json & j, const quicktype::PerClassLoadoutSlots & x) {
        j = json::object();
        j["Soldier"] = x.soldier;
        j["Heavy"] = x.heavy;
        j["Pyro"] = x.pyro;
        j["Engineer"] = x.engineer;
        j["Demoman"] = x.demoman;
    }

    inline void from_json(const json & j, quicktype::AdditionalHiddenBodygroups& x) {
        x.hat = quicktype::get_optional<int64_t>(j, "hat");
        x.headphones = quicktype::get_optional<int64_t>(j, "headphones");
        x.head = quicktype::get_optional<int64_t>(j, "head");
        x.grenades = quicktype::get_optional<int64_t>(j, "grenades");
    }

    inline void to_json(json & j, const quicktype::AdditionalHiddenBodygroups & x) {
        j = json::object();
        j["hat"] = x.hat;
        j["headphones"] = x.headphones;
        j["head"] = x.head;
        j["grenades"] = x.grenades;
    }

    inline void from_json(const json & j, quicktype::Style& x) {
        x.name = j.at("name").get<std::string>();
        x.additional_hidden_bodygroups = quicktype::get_optional<quicktype::AdditionalHiddenBodygroups>(j, "additional_hidden_bodygroups");
    }

    inline void to_json(json & j, const quicktype::Style & x) {
        j = json::object();
        j["name"] = x.name;
        j["additional_hidden_bodygroups"] = x.additional_hidden_bodygroups;
    }

    inline void from_json(const json & j, quicktype::Tool& x) {
        x.type = j.at("type").get<std::string>();
        x.usage_capabilities = quicktype::get_optional<std::map<std::string, bool>>(j, "usage_capabilities");
        x.use_string = quicktype::get_optional<std::string>(j, "use_string");
        x.restriction = quicktype::get_optional<std::string>(j, "restriction");
    }

    inline void to_json(json & j, const quicktype::Tool & x) {
        j = json::object();
        j["type"] = x.type;
        j["usage_capabilities"] = x.usage_capabilities;
        j["use_string"] = x.use_string;
        j["restriction"] = x.restriction;
    }

    inline void from_json(const json & j, quicktype::Item& x) {
        x.name = j.at("name").get<std::string>();
        x.defindex = j.at("defindex").get<int64_t>();
        x.item_class = j.at("item_class").get<quicktype::ItemClass>();
        x.item_type_name = j.at("item_type_name").get<std::string>();
        x.item_name = j.at("item_name").get<std::string>();
        x.proper_name = j.at("proper_name").get<bool>();
        x.item_slot = quicktype::get_optional<quicktype::ItemSlot>(j, "item_slot");
        x.model_player = quicktype::get_optional<std::string>(j, "model_player");
        x.item_quality = j.at("item_quality").get<int64_t>();
        x.image_inventory = quicktype::get_optional<std::string>(j, "image_inventory");
        x.min_ilevel = j.at("min_ilevel").get<int64_t>();
        x.max_ilevel = j.at("max_ilevel").get<int64_t>();
        x.image_url = quicktype::get_optional<std::string>(j, "image_url");
        x.image_url_large = quicktype::get_optional<std::string>(j, "image_url_large");
        x.craft_class = quicktype::get_optional<quicktype::Craft>(j, "craft_class");
        x.craft_material_type = quicktype::get_optional<quicktype::Craft>(j, "craft_material_type");
        x.capabilities = j.at("capabilities").get<std::map<std::string, bool>>();
        x.used_by_classes = quicktype::get_optional<std::vector<quicktype::UsedByClass>>(j, "used_by_classes");
        x.item_description = quicktype::get_optional<std::string>(j, "item_description");
        x.styles = quicktype::get_optional<std::vector<quicktype::Style>>(j, "styles");
        x.attributes = quicktype::get_optional<std::vector<quicktype::Attribute>>(j, "attributes");
        x.drop_type = quicktype::get_optional<quicktype::DropType>(j, "drop_type");
        x.item_set = quicktype::get_optional<std::string>(j, "item_set");
        x.holiday_restriction = quicktype::get_optional<quicktype::HolidayRestriction>(j, "holiday_restriction");
        x.per_class_loadout_slots = quicktype::get_optional<quicktype::PerClassLoadoutSlots>(j, "per_class_loadout_slots");
        x.tool = quicktype::get_optional<quicktype::Tool>(j, "tool");
    }

    inline void to_json(json & j, const quicktype::Item & x) {
        j = json::object();
        j["name"] = x.name;
        j["defindex"] = x.defindex;
        j["item_class"] = x.item_class;
        j["item_type_name"] = x.item_type_name;
        j["item_name"] = x.item_name;
        j["proper_name"] = x.proper_name;
        j["item_slot"] = x.item_slot;
        j["model_player"] = x.model_player;
        j["item_quality"] = x.item_quality;
        j["image_inventory"] = x.image_inventory;
        j["min_ilevel"] = x.min_ilevel;
        j["max_ilevel"] = x.max_ilevel;
        j["image_url"] = x.image_url;
        j["image_url_large"] = x.image_url_large;
        j["craft_class"] = x.craft_class;
        j["craft_material_type"] = x.craft_material_type;
        j["capabilities"] = x.capabilities;
        j["used_by_classes"] = x.used_by_classes;
        j["item_description"] = x.item_description;
        j["styles"] = x.styles;
        j["attributes"] = x.attributes;
        j["drop_type"] = x.drop_type;
        j["item_set"] = x.item_set;
        j["holiday_restriction"] = x.holiday_restriction;
        j["per_class_loadout_slots"] = x.per_class_loadout_slots;
        j["tool"] = x.tool;
    }

    inline void from_json(const json & j, quicktype::Result& x) {
        x.status = j.at("status").get<int64_t>();
        x.items_game_url = j.at("items_game_url").get<std::string>();
        x.items = j.at("items").get<std::vector<quicktype::Item>>();
        x.next = quicktype::get_optional<int64_t>(j, "next");
    }

    inline void to_json(json & j, const quicktype::Result & x) {
        j = json::object();
        j["status"] = x.status;
        j["items_game_url"] = x.items_game_url;
        j["items"] = x.items;
        j["next"] = x.next;
    }

    inline void from_json(const json & j, quicktype::SchemaElement& x) {
        x.result = j.at("result").get<quicktype::Result>();
    }

    inline void to_json(json & j, const quicktype::SchemaElement & x) {
        j = json::object();
        j["result"] = x.result;
    }

    inline void from_json(const json & j, quicktype::Craft & x) {
        if (j == "craft_bar") x = quicktype::Craft::CRAFT_BAR;
        else if (j == "craft_material_burned") x = quicktype::Craft::CRAFT_MATERIAL_BURNED;
        else if (j == "craft_material_voodoocursed") x = quicktype::Craft::CRAFT_MATERIAL_VOODOOCURSED;
        else if (j == "craft_token") x = quicktype::Craft::CRAFT_TOKEN;
        else if (j == "") x = quicktype::Craft::EMPTY;
        else if (j == "hat") x = quicktype::Craft::HAT;
        else if (j == "haunted_hat") x = quicktype::Craft::HAUNTED_HAT;
        else if (j == "strangepart") x = quicktype::Craft::STRANGEPART;
        else if (j == "supply_crate") x = quicktype::Craft::SUPPLY_CRATE;
        else if (j == "tool") x = quicktype::Craft::TOOL;
        else if (j == "weapon") x = quicktype::Craft::WEAPON;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const quicktype::Craft & x) {
        switch (x) {
            case quicktype::Craft::CRAFT_BAR: j = "craft_bar"; break;
            case quicktype::Craft::CRAFT_MATERIAL_BURNED: j = "craft_material_burned"; break;
            case quicktype::Craft::CRAFT_MATERIAL_VOODOOCURSED: j = "craft_material_voodoocursed"; break;
            case quicktype::Craft::CRAFT_TOKEN: j = "craft_token"; break;
            case quicktype::Craft::EMPTY: j = ""; break;
            case quicktype::Craft::HAT: j = "hat"; break;
            case quicktype::Craft::HAUNTED_HAT: j = "haunted_hat"; break;
            case quicktype::Craft::STRANGEPART: j = "strangepart"; break;
            case quicktype::Craft::SUPPLY_CRATE: j = "supply_crate"; break;
            case quicktype::Craft::TOOL: j = "tool"; break;
            case quicktype::Craft::WEAPON: j = "weapon"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, quicktype::DropType & x) {
        if (j == "drop") x = quicktype::DropType::DROP;
        else if (j == "none") x = quicktype::DropType::NONE;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const quicktype::DropType & x) {
        switch (x) {
            case quicktype::DropType::DROP: j = "drop"; break;
            case quicktype::DropType::NONE: j = "none"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, quicktype::HolidayRestriction & x) {
        if (j == "birthday") x = quicktype::HolidayRestriction::BIRTHDAY;
        else if (j == "christmas") x = quicktype::HolidayRestriction::CHRISTMAS;
        else if (j == "halloween") x = quicktype::HolidayRestriction::HALLOWEEN;
        else if (j == "halloween_or_fullmoon") x = quicktype::HolidayRestriction::HALLOWEEN_OR_FULLMOON;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const quicktype::HolidayRestriction & x) {
        switch (x) {
            case quicktype::HolidayRestriction::BIRTHDAY: j = "birthday"; break;
            case quicktype::HolidayRestriction::CHRISTMAS: j = "christmas"; break;
            case quicktype::HolidayRestriction::HALLOWEEN: j = "halloween"; break;
            case quicktype::HolidayRestriction::HALLOWEEN_OR_FULLMOON: j = "halloween_or_fullmoon"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, quicktype::ItemClass & x) {
        static std::unordered_map<std::string, quicktype::ItemClass> enumValues {
                {"bundle", quicktype::ItemClass::BUNDLE},
                {"class_token", quicktype::ItemClass::CLASS_TOKEN},
                {"craft_item", quicktype::ItemClass::CRAFT_ITEM},
                {"map_token", quicktype::ItemClass::MAP_TOKEN},
                {"no_entity", quicktype::ItemClass::NO_ENTITY},
                {"saxxy", quicktype::ItemClass::SAXXY},
                {"slot_token", quicktype::ItemClass::SLOT_TOKEN},
                {"supply_crate", quicktype::ItemClass::SUPPLY_CRATE},
                {"tf_powerup_bottle", quicktype::ItemClass::TF_POWERUP_BOTTLE},
                {"tf_weapon_bat", quicktype::ItemClass::TF_WEAPON_BAT},
                {"tf_weapon_bat_fish", quicktype::ItemClass::TF_WEAPON_BAT_FISH},
                {"tf_weapon_bat_giftwrap", quicktype::ItemClass::TF_WEAPON_BAT_GIFTWRAP},
                {"tf_weapon_bat_wood", quicktype::ItemClass::TF_WEAPON_BAT_WOOD},
                {"tf_weapon_bonesaw", quicktype::ItemClass::TF_WEAPON_BONESAW},
                {"tf_weapon_bottle", quicktype::ItemClass::TF_WEAPON_BOTTLE},
                {"tf_weapon_breakable_sign", quicktype::ItemClass::TF_WEAPON_BREAKABLE_SIGN},
                {"tf_weapon_buff_item", quicktype::ItemClass::TF_WEAPON_BUFF_ITEM},
                {"tf_weapon_builder", quicktype::ItemClass::TF_WEAPON_BUILDER},
                {"tf_weapon_cannon", quicktype::ItemClass::TF_WEAPON_CANNON},
                {"tf_weapon_charged_smg", quicktype::ItemClass::TF_WEAPON_CHARGED_SMG},
                {"tf_weapon_cleaver", quicktype::ItemClass::TF_WEAPON_CLEAVER},
                {"tf_weapon_club", quicktype::ItemClass::TF_WEAPON_CLUB},
                {"tf_weapon_compound_bow", quicktype::ItemClass::TF_WEAPON_COMPOUND_BOW},
                {"tf_weapon_crossbow", quicktype::ItemClass::TF_WEAPON_CROSSBOW},
                {"tf_weapon_drg_pomson", quicktype::ItemClass::TF_WEAPON_DRG_POMSON},
                {"tf_weapon_fireaxe", quicktype::ItemClass::TF_WEAPON_FIREAXE},
                {"tf_weapon_fists", quicktype::ItemClass::TF_WEAPON_FISTS},
                {"tf_weapon_flamethrower", quicktype::ItemClass::TF_WEAPON_FLAMETHROWER},
                {"tf_weapon_flaregun", quicktype::ItemClass::TF_WEAPON_FLAREGUN},
                {"tf_weapon_flaregun_revenge", quicktype::ItemClass::TF_WEAPON_FLAREGUN_REVENGE},
                {"tf_weapon_grapplinghook", quicktype::ItemClass::TF_WEAPON_GRAPPLINGHOOK},
                {"tf_weapon_grenadelauncher", quicktype::ItemClass::TF_WEAPON_GRENADELAUNCHER},
                {"tf_weapon_handgun_scout_primary", quicktype::ItemClass::TF_WEAPON_HANDGUN_SCOUT_PRIMARY},
                {"tf_weapon_handgun_scout_secondary", quicktype::ItemClass::TF_WEAPON_HANDGUN_SCOUT_SECONDARY},
                {"tf_weapon_invis", quicktype::ItemClass::TF_WEAPON_INVIS},
                {"tf_weapon_jar", quicktype::ItemClass::TF_WEAPON_JAR},
                {"tf_weapon_jar_gas", quicktype::ItemClass::TF_WEAPON_JAR_GAS},
                {"tf_weapon_jar_milk", quicktype::ItemClass::TF_WEAPON_JAR_MILK},
                {"tf_weapon_katana", quicktype::ItemClass::TF_WEAPON_KATANA},
                {"tf_weapon_knife", quicktype::ItemClass::TF_WEAPON_KNIFE},
                {"tf_weapon_laser_pointer", quicktype::ItemClass::TF_WEAPON_LASER_POINTER},
                {"tf_weapon_lunchbox", quicktype::ItemClass::TF_WEAPON_LUNCHBOX},
                {"tf_weapon_lunchbox_drink", quicktype::ItemClass::TF_WEAPON_LUNCHBOX_DRINK},
                {"tf_weapon_mechanical_arm", quicktype::ItemClass::TF_WEAPON_MECHANICAL_ARM},
                {"tf_weapon_medigun", quicktype::ItemClass::TF_WEAPON_MEDIGUN},
                {"tf_weapon_minigun", quicktype::ItemClass::TF_WEAPON_MINIGUN},
                {"tf_weapon_parachute", quicktype::ItemClass::TF_WEAPON_PARACHUTE},
                {"tf_weapon_particle_cannon", quicktype::ItemClass::TF_WEAPON_PARTICLE_CANNON},
                {"tf_weapon_passtime_gun", quicktype::ItemClass::TF_WEAPON_PASSTIME_GUN},
                {"tf_weapon_pda_engineer_build", quicktype::ItemClass::TF_WEAPON_PDA_ENGINEER_BUILD},
                {"tf_weapon_pda_engineer_destroy", quicktype::ItemClass::TF_WEAPON_PDA_ENGINEER_DESTROY},
                {"tf_weapon_pda_spy", quicktype::ItemClass::TF_WEAPON_PDA_SPY},
                {"tf_weapon_pep_brawler_blaster", quicktype::ItemClass::TF_WEAPON_PEP_BRAWLER_BLASTER},
                {"tf_weapon_pipebomblauncher", quicktype::ItemClass::TF_WEAPON_PIPEBOMBLAUNCHER},
                {"tf_weapon_pistol", quicktype::ItemClass::TF_WEAPON_PISTOL},
                {"tf_weapon_raygun", quicktype::ItemClass::TF_WEAPON_RAYGUN},
                {"tf_weapon_revolver", quicktype::ItemClass::TF_WEAPON_REVOLVER},
                {"tf_weapon_robot_arm", quicktype::ItemClass::TF_WEAPON_ROBOT_ARM},
                {"tf_weapon_rocketlauncher", quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER},
                {"tf_weapon_rocketlauncher_airstrike", quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER_AIRSTRIKE},
                {"tf_weapon_rocketlauncher_directhit", quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT},
                {"tf_weapon_rocketlauncher_fireball", quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER_FIREBALL},
                {"tf_weapon_rocketpack", quicktype::ItemClass::TF_WEAPON_ROCKETPACK},
                {"tf_weapon_sapper", quicktype::ItemClass::TF_WEAPON_SAPPER},
                {"tf_weapon_scattergun", quicktype::ItemClass::TF_WEAPON_SCATTERGUN},
                {"tf_weapon_sentry_revenge", quicktype::ItemClass::TF_WEAPON_SENTRY_REVENGE},
                {"tf_weapon_shotgun", quicktype::ItemClass::TF_WEAPON_SHOTGUN},
                {"tf_weapon_shotgun_building_rescue", quicktype::ItemClass::TF_WEAPON_SHOTGUN_BUILDING_RESCUE},
                {"tf_weapon_shotgun_primary", quicktype::ItemClass::TF_WEAPON_SHOTGUN_PRIMARY},
                {"tf_weapon_shovel", quicktype::ItemClass::TF_WEAPON_SHOVEL},
                {"tf_weapon_slap", quicktype::ItemClass::TF_WEAPON_SLAP},
                {"tf_weapon_smg", quicktype::ItemClass::TF_WEAPON_SMG},
                {"tf_weapon_sniperrifle", quicktype::ItemClass::TF_WEAPON_SNIPERRIFLE},
                {"tf_weapon_sniperrifle_classic", quicktype::ItemClass::TF_WEAPON_SNIPERRIFLE_CLASSIC},
                {"tf_weapon_sniperrifle_decap", quicktype::ItemClass::TF_WEAPON_SNIPERRIFLE_DECAP},
                {"tf_weapon_soda_popper", quicktype::ItemClass::TF_WEAPON_SODA_POPPER},
                {"tf_weapon_spellbook", quicktype::ItemClass::TF_WEAPON_SPELLBOOK},
                {"tf_weapon_stickbomb", quicktype::ItemClass::TF_WEAPON_STICKBOMB},
                {"tf_weapon_sword", quicktype::ItemClass::TF_WEAPON_SWORD},
                {"tf_weapon_syringegun_medic", quicktype::ItemClass::TF_WEAPON_SYRINGEGUN_MEDIC},
                {"tf_weapon_wrench", quicktype::ItemClass::TF_WEAPON_WRENCH},
                {"tf_wearable", quicktype::ItemClass::TF_WEARABLE},
                {"tf_wearable_campaign_item", quicktype::ItemClass::TF_WEARABLE_CAMPAIGN_ITEM},
                {"tf_wearable_demoshield", quicktype::ItemClass::TF_WEARABLE_DEMOSHIELD},
                {"tf_wearable_levelable_item", quicktype::ItemClass::TF_WEARABLE_LEVELABLE_ITEM},
                {"tf_wearable_razorback", quicktype::ItemClass::TF_WEARABLE_RAZORBACK},
                {"tool", quicktype::ItemClass::TOOL},
                {"upgrade", quicktype::ItemClass::UPGRADE},
        };
        auto iter = enumValues.find(j.get<std::string>());
        if (iter != enumValues.end()) {
            x = iter->second;
        }
    }

    inline void to_json(json & j, const quicktype::ItemClass & x) {
        switch (x) {
            case quicktype::ItemClass::BUNDLE: j = "bundle"; break;
            case quicktype::ItemClass::CLASS_TOKEN: j = "class_token"; break;
            case quicktype::ItemClass::CRAFT_ITEM: j = "craft_item"; break;
            case quicktype::ItemClass::MAP_TOKEN: j = "map_token"; break;
            case quicktype::ItemClass::NO_ENTITY: j = "no_entity"; break;
            case quicktype::ItemClass::SAXXY: j = "saxxy"; break;
            case quicktype::ItemClass::SLOT_TOKEN: j = "slot_token"; break;
            case quicktype::ItemClass::SUPPLY_CRATE: j = "supply_crate"; break;
            case quicktype::ItemClass::TF_POWERUP_BOTTLE: j = "tf_powerup_bottle"; break;
            case quicktype::ItemClass::TF_WEAPON_BAT: j = "tf_weapon_bat"; break;
            case quicktype::ItemClass::TF_WEAPON_BAT_FISH: j = "tf_weapon_bat_fish"; break;
            case quicktype::ItemClass::TF_WEAPON_BAT_GIFTWRAP: j = "tf_weapon_bat_giftwrap"; break;
            case quicktype::ItemClass::TF_WEAPON_BAT_WOOD: j = "tf_weapon_bat_wood"; break;
            case quicktype::ItemClass::TF_WEAPON_BONESAW: j = "tf_weapon_bonesaw"; break;
            case quicktype::ItemClass::TF_WEAPON_BOTTLE: j = "tf_weapon_bottle"; break;
            case quicktype::ItemClass::TF_WEAPON_BREAKABLE_SIGN: j = "tf_weapon_breakable_sign"; break;
            case quicktype::ItemClass::TF_WEAPON_BUFF_ITEM: j = "tf_weapon_buff_item"; break;
            case quicktype::ItemClass::TF_WEAPON_BUILDER: j = "tf_weapon_builder"; break;
            case quicktype::ItemClass::TF_WEAPON_CANNON: j = "tf_weapon_cannon"; break;
            case quicktype::ItemClass::TF_WEAPON_CHARGED_SMG: j = "tf_weapon_charged_smg"; break;
            case quicktype::ItemClass::TF_WEAPON_CLEAVER: j = "tf_weapon_cleaver"; break;
            case quicktype::ItemClass::TF_WEAPON_CLUB: j = "tf_weapon_club"; break;
            case quicktype::ItemClass::TF_WEAPON_COMPOUND_BOW: j = "tf_weapon_compound_bow"; break;
            case quicktype::ItemClass::TF_WEAPON_CROSSBOW: j = "tf_weapon_crossbow"; break;
            case quicktype::ItemClass::TF_WEAPON_DRG_POMSON: j = "tf_weapon_drg_pomson"; break;
            case quicktype::ItemClass::TF_WEAPON_FIREAXE: j = "tf_weapon_fireaxe"; break;
            case quicktype::ItemClass::TF_WEAPON_FISTS: j = "tf_weapon_fists"; break;
            case quicktype::ItemClass::TF_WEAPON_FLAMETHROWER: j = "tf_weapon_flamethrower"; break;
            case quicktype::ItemClass::TF_WEAPON_FLAREGUN: j = "tf_weapon_flaregun"; break;
            case quicktype::ItemClass::TF_WEAPON_FLAREGUN_REVENGE: j = "tf_weapon_flaregun_revenge"; break;
            case quicktype::ItemClass::TF_WEAPON_GRAPPLINGHOOK: j = "tf_weapon_grapplinghook"; break;
            case quicktype::ItemClass::TF_WEAPON_GRENADELAUNCHER: j = "tf_weapon_grenadelauncher"; break;
            case quicktype::ItemClass::TF_WEAPON_HANDGUN_SCOUT_PRIMARY: j = "tf_weapon_handgun_scout_primary"; break;
            case quicktype::ItemClass::TF_WEAPON_HANDGUN_SCOUT_SECONDARY: j = "tf_weapon_handgun_scout_secondary"; break;
            case quicktype::ItemClass::TF_WEAPON_INVIS: j = "tf_weapon_invis"; break;
            case quicktype::ItemClass::TF_WEAPON_JAR: j = "tf_weapon_jar"; break;
            case quicktype::ItemClass::TF_WEAPON_JAR_GAS: j = "tf_weapon_jar_gas"; break;
            case quicktype::ItemClass::TF_WEAPON_JAR_MILK: j = "tf_weapon_jar_milk"; break;
            case quicktype::ItemClass::TF_WEAPON_KATANA: j = "tf_weapon_katana"; break;
            case quicktype::ItemClass::TF_WEAPON_KNIFE: j = "tf_weapon_knife"; break;
            case quicktype::ItemClass::TF_WEAPON_LASER_POINTER: j = "tf_weapon_laser_pointer"; break;
            case quicktype::ItemClass::TF_WEAPON_LUNCHBOX: j = "tf_weapon_lunchbox"; break;
            case quicktype::ItemClass::TF_WEAPON_LUNCHBOX_DRINK: j = "tf_weapon_lunchbox_drink"; break;
            case quicktype::ItemClass::TF_WEAPON_MECHANICAL_ARM: j = "tf_weapon_mechanical_arm"; break;
            case quicktype::ItemClass::TF_WEAPON_MEDIGUN: j = "tf_weapon_medigun"; break;
            case quicktype::ItemClass::TF_WEAPON_MINIGUN: j = "tf_weapon_minigun"; break;
            case quicktype::ItemClass::TF_WEAPON_PARACHUTE: j = "tf_weapon_parachute"; break;
            case quicktype::ItemClass::TF_WEAPON_PARTICLE_CANNON: j = "tf_weapon_particle_cannon"; break;
            case quicktype::ItemClass::TF_WEAPON_PASSTIME_GUN: j = "tf_weapon_passtime_gun"; break;
            case quicktype::ItemClass::TF_WEAPON_PDA_ENGINEER_BUILD: j = "tf_weapon_pda_engineer_build"; break;
            case quicktype::ItemClass::TF_WEAPON_PDA_ENGINEER_DESTROY: j = "tf_weapon_pda_engineer_destroy"; break;
            case quicktype::ItemClass::TF_WEAPON_PDA_SPY: j = "tf_weapon_pda_spy"; break;
            case quicktype::ItemClass::TF_WEAPON_PEP_BRAWLER_BLASTER: j = "tf_weapon_pep_brawler_blaster"; break;
            case quicktype::ItemClass::TF_WEAPON_PIPEBOMBLAUNCHER: j = "tf_weapon_pipebomblauncher"; break;
            case quicktype::ItemClass::TF_WEAPON_PISTOL: j = "tf_weapon_pistol"; break;
            case quicktype::ItemClass::TF_WEAPON_RAYGUN: j = "tf_weapon_raygun"; break;
            case quicktype::ItemClass::TF_WEAPON_REVOLVER: j = "tf_weapon_revolver"; break;
            case quicktype::ItemClass::TF_WEAPON_ROBOT_ARM: j = "tf_weapon_robot_arm"; break;
            case quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER: j = "tf_weapon_rocketlauncher"; break;
            case quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER_AIRSTRIKE: j = "tf_weapon_rocketlauncher_airstrike"; break;
            case quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT: j = "tf_weapon_rocketlauncher_directhit"; break;
            case quicktype::ItemClass::TF_WEAPON_ROCKETLAUNCHER_FIREBALL: j = "tf_weapon_rocketlauncher_fireball"; break;
            case quicktype::ItemClass::TF_WEAPON_ROCKETPACK: j = "tf_weapon_rocketpack"; break;
            case quicktype::ItemClass::TF_WEAPON_SAPPER: j = "tf_weapon_sapper"; break;
            case quicktype::ItemClass::TF_WEAPON_SCATTERGUN: j = "tf_weapon_scattergun"; break;
            case quicktype::ItemClass::TF_WEAPON_SENTRY_REVENGE: j = "tf_weapon_sentry_revenge"; break;
            case quicktype::ItemClass::TF_WEAPON_SHOTGUN: j = "tf_weapon_shotgun"; break;
            case quicktype::ItemClass::TF_WEAPON_SHOTGUN_BUILDING_RESCUE: j = "tf_weapon_shotgun_building_rescue"; break;
            case quicktype::ItemClass::TF_WEAPON_SHOTGUN_PRIMARY: j = "tf_weapon_shotgun_primary"; break;
            case quicktype::ItemClass::TF_WEAPON_SHOVEL: j = "tf_weapon_shovel"; break;
            case quicktype::ItemClass::TF_WEAPON_SLAP: j = "tf_weapon_slap"; break;
            case quicktype::ItemClass::TF_WEAPON_SMG: j = "tf_weapon_smg"; break;
            case quicktype::ItemClass::TF_WEAPON_SNIPERRIFLE: j = "tf_weapon_sniperrifle"; break;
            case quicktype::ItemClass::TF_WEAPON_SNIPERRIFLE_CLASSIC: j = "tf_weapon_sniperrifle_classic"; break;
            case quicktype::ItemClass::TF_WEAPON_SNIPERRIFLE_DECAP: j = "tf_weapon_sniperrifle_decap"; break;
            case quicktype::ItemClass::TF_WEAPON_SODA_POPPER: j = "tf_weapon_soda_popper"; break;
            case quicktype::ItemClass::TF_WEAPON_SPELLBOOK: j = "tf_weapon_spellbook"; break;
            case quicktype::ItemClass::TF_WEAPON_STICKBOMB: j = "tf_weapon_stickbomb"; break;
            case quicktype::ItemClass::TF_WEAPON_SWORD: j = "tf_weapon_sword"; break;
            case quicktype::ItemClass::TF_WEAPON_SYRINGEGUN_MEDIC: j = "tf_weapon_syringegun_medic"; break;
            case quicktype::ItemClass::TF_WEAPON_WRENCH: j = "tf_weapon_wrench"; break;
            case quicktype::ItemClass::TF_WEARABLE: j = "tf_wearable"; break;
            case quicktype::ItemClass::TF_WEARABLE_CAMPAIGN_ITEM: j = "tf_wearable_campaign_item"; break;
            case quicktype::ItemClass::TF_WEARABLE_DEMOSHIELD: j = "tf_wearable_demoshield"; break;
            case quicktype::ItemClass::TF_WEARABLE_LEVELABLE_ITEM: j = "tf_wearable_levelable_item"; break;
            case quicktype::ItemClass::TF_WEARABLE_RAZORBACK: j = "tf_wearable_razorback"; break;
            case quicktype::ItemClass::TOOL: j = "tool"; break;
            case quicktype::ItemClass::UPGRADE: j = "upgrade"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, quicktype::ItemSlot & x) {
        if (j == "action") x = quicktype::ItemSlot::ACTION;
        else if (j == "building") x = quicktype::ItemSlot::BUILDING;
        else if (j == "melee") x = quicktype::ItemSlot::MELEE;
        else if (j == "misc") x = quicktype::ItemSlot::MISC;
        else if (j == "pda") x = quicktype::ItemSlot::PDA;
        else if (j == "pda2") x = quicktype::ItemSlot::PDA2;
        else if (j == "primary") x = quicktype::ItemSlot::PRIMARY;
        else if (j == "secondary") x = quicktype::ItemSlot::SECONDARY;
        else if (j == "taunt") x = quicktype::ItemSlot::TAUNT;
        else if (j == "utility") x = quicktype::ItemSlot::UTILITY;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const quicktype::ItemSlot & x) {
        switch (x) {
            case quicktype::ItemSlot::ACTION: j = "action"; break;
            case quicktype::ItemSlot::BUILDING: j = "building"; break;
            case quicktype::ItemSlot::MELEE: j = "melee"; break;
            case quicktype::ItemSlot::MISC: j = "misc"; break;
            case quicktype::ItemSlot::PDA: j = "pda"; break;
            case quicktype::ItemSlot::PDA2: j = "pda2"; break;
            case quicktype::ItemSlot::PRIMARY: j = "primary"; break;
            case quicktype::ItemSlot::SECONDARY: j = "secondary"; break;
            case quicktype::ItemSlot::TAUNT: j = "taunt"; break;
            case quicktype::ItemSlot::UTILITY: j = "utility"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, quicktype::UsedByClass & x) {
        if (j == "Demoman") x = quicktype::UsedByClass::DEMOMAN;
        else if (j == "Engineer") x = quicktype::UsedByClass::ENGINEER;
        else if (j == "Heavy") x = quicktype::UsedByClass::HEAVY;
        else if (j == "Medic") x = quicktype::UsedByClass::MEDIC;
        else if (j == "Pyro") x = quicktype::UsedByClass::PYRO;
        else if (j == "Scout") x = quicktype::UsedByClass::SCOUT;
        else if (j == "Sniper") x = quicktype::UsedByClass::SNIPER;
        else if (j == "Soldier") x = quicktype::UsedByClass::SOLDIER;
        else if (j == "Spy") x = quicktype::UsedByClass::SPY;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const quicktype::UsedByClass & x) {
        switch (x) {
            case quicktype::UsedByClass::DEMOMAN: j = "Demoman"; break;
            case quicktype::UsedByClass::ENGINEER: j = "Engineer"; break;
            case quicktype::UsedByClass::HEAVY: j = "Heavy"; break;
            case quicktype::UsedByClass::MEDIC: j = "Medic"; break;
            case quicktype::UsedByClass::PYRO: j = "Pyro"; break;
            case quicktype::UsedByClass::SCOUT: j = "Scout"; break;
            case quicktype::UsedByClass::SNIPER: j = "Sniper"; break;
            case quicktype::UsedByClass::SOLDIER: j = "Soldier"; break;
            case quicktype::UsedByClass::SPY: j = "Spy"; break;
            default: throw "This should not happen";
        }
    }
}
