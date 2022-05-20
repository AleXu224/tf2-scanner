#ifndef BPSCANNER_ITEM_HPP
#define BPSCANNER_ITEM_HPP

#include "../json_schemas/InventoryData.hpp"
#include "string"
#include "imgui.h"
#include "map"

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
    {QUALITY::COLLECTORS, "Collectors"},
    {QUALITY::DECORATED, "Decorated"}
};

enum class TF2CURRENCY {
    KEYS = 1,
    METAL = 2,
    HATS = 3
};

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
    {WEAR::FACTORY_NEW, "Factory New"}
};

#define imageUrlPrefix "https://steamcommunity-a.akamaihd.net/economy/image/"

#define effectUrlPrefix "https://backpack.tf/images/440/particles/"
#define effecturlSuffix "_94x94.png"

class Item {
private:
    std::string workingName;
public:
    bool tradable = true;

    int defindex;
    std::string name;
    std::string nameSimple; // name without prefixes or suffixes
    bool craftable = true;
    bool australium = false;
    KILLSTREAK killstreak = KILLSTREAK::NONE;
    QUALITY quality = QUALITY::UNIQUE;
    QUALITY qualitySecondary = QUALITY::NONE;

    bool properName = false; // wether the name has "The" in it or not

    bool isCrate = false;

    std::string effectName;
    int effectID = -1;

    int crateID = -1;

    std::string imageUrl; // This only contains the last part of the url, not the whole thing

    int skinID = -1;
    WEAR skinWear = WEAR::NONE;

    int targetDefindex;
    std::string targetName;
    TF2CURRENCY currency;
    float price;

    bool fail = false;

    explicit Item(JsonInventory::InventoryDescription &itemData);

    void ToConsole() const;
};

#endif //BPSCANNER_ITEM_HPP