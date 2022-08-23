#pragma once
#include "../json_schemas/SteamInventory.hpp"
#include "imgui.h"
#include "map"
#include "string"

enum class KILLSTREAK {
    NONE = 0,
    BASIC = 1,
    SPECIALIZED = 2,
    PROFFESIONAL = 3
};

enum class QUALITY {
    NONE = -1,
    NORMAL = 0,
    GENIUNE = 1,
    VINTAGE = 3,
    UNUSUAL = 5,
    UNIQUE = 6,
    COMMUNITY = 7,
    VALVE = 8,
    SELF_MADE = 9,
    STRANGE = 11,
    HAUNTED = 13,
    COLLECTORS = 14,
    DECORATED = 15
};

const std::map<QUALITY, std::string> QUALITY_STRINGS = {
    {QUALITY::NORMAL, "Normal"},
    {QUALITY::GENIUNE, "Genuine"},
    {QUALITY::VINTAGE, "Vintage"},
    {QUALITY::UNUSUAL, "Unusual"},
    {QUALITY::UNIQUE, "Unique"},
    {QUALITY::COMMUNITY, "Community"},
    {QUALITY::VALVE, "Valve"},
    {QUALITY::SELF_MADE, "Self-Made"},
    {QUALITY::STRANGE, "Strange"},
    {QUALITY::HAUNTED, "Haunted"},
    {QUALITY::COLLECTORS, "Collector's"},
    {QUALITY::DECORATED, "Decorated"}};

const std::map<QUALITY, ImColor> QUALITY_COLORS = {
    {QUALITY::NORMAL, ImColor(178, 178, 178)},
    {QUALITY::GENIUNE, ImColor(77, 116, 85)},
    {QUALITY::VINTAGE, ImColor(71, 98, 145)},
    {QUALITY::UNUSUAL, ImColor(134, 80, 172)},
    {QUALITY::UNIQUE, ImColor(255, 215, 0)},
    {QUALITY::COMMUNITY, ImColor(112, 176, 74)},
    {QUALITY::VALVE, ImColor(165, 15, 121)},
    {QUALITY::SELF_MADE, ImColor(112, 176, 74)},
    {QUALITY::STRANGE, ImColor(207, 106, 50)},
    {QUALITY::HAUNTED, ImColor(56, 243, 171)},
    {QUALITY::COLLECTORS, ImColor(170, 0, 0)},
    {QUALITY::DECORATED, ImColor(250, 250, 250)}};

enum class TF2CURRENCY {
    NONE = -1,
    KEYS = 1,
    METAL = 2,
    HATS = 3,
    USD = 4
};

const std::map<TF2CURRENCY, std::string> TF2CURRENCY_STRINGS = {
    {TF2CURRENCY::NONE, "None"},
    {TF2CURRENCY::KEYS, "Keys"},
    {TF2CURRENCY::METAL, "Ref"},
    {TF2CURRENCY::HATS, "Hats"},
    {TF2CURRENCY::USD, "USD"}};

enum class WEAR {
    NONE = -1,
    BATTLE_SCARRED,
    WELL_WORN,
    FIELD_TESTED,
    MINIMAL_WEAR,
    FACTORY_NEW
};

const std::map<WEAR, std::string> WEAR_STRINGS = {
    {WEAR::NONE, "None"},
    {WEAR::BATTLE_SCARRED, "Battle Scarred"},
    {WEAR::WELL_WORN, "Well-Worn"},
    {WEAR::FIELD_TESTED, "Field-Tested"},
    {WEAR::MINIMAL_WEAR, "Minimal Wear"},
    {WEAR::FACTORY_NEW, "Factory New"}};

#define imageUrlPrefix "https://steamcommunity-a.akamaihd.net/economy/image/"

#define effectUrlPrefix "https://backpack.tf/images/440/particles/"
#define effecturlSuffix "_94x94.png"

class Item {
   private:
    std::string workingName;

   public:
    bool tradable = true;

    int defindex = -1;
    std::string name;
    std::string nameSimple;  // name without prefixes or suffixes
    bool craftable = true;
    bool australium = false;
    KILLSTREAK killstreak = KILLSTREAK::NONE;
    QUALITY quality = QUALITY::UNIQUE;
    QUALITY qualitySecondary = QUALITY::NONE;

    bool properName = false;  // wether the name has "The" in it or not

    bool isCrate = false;
    int crateID = -1;

    int effectID = -1;
    std::string effectName;

    std::string imageUrl;  // This only contains the last part of the url, not the whole thing

    int skinID = -1;
    WEAR skinWear = WEAR::NONE;

    int toolDefindex = -1;
    std::string toolName;

    int recipeDefindex = -1;
    std::string recipeName;

    bool isFestivized = false;

    TF2CURRENCY currency = TF2CURRENCY::NONE;
    float price = -1;

    bool fail = false;

    explicit Item(JSON::SteamInventory::Description &itemData);

    float getKeyPrice();

    void ToConsole() const;
};