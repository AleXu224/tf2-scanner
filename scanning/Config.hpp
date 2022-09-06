#ifndef BPSCANNER_CONFIG_HPP
#define BPSCANNER_CONFIG_HPP

#include "../components/Console.hpp"
#include "../json_schemas/TFEffects.hpp"
#include "../json_schemas/BptfPrices.hpp"
#include "../json_schemas/ItemSchema.hpp"
#include "../json_schemas/TfSkins.hpp"
#include "../json_schemas/TFMarket.hpp"
#include "cpr/cpr.h"
#include "filesystem"
#include "iostream"
#include "sstream"

class Config {
   public:
    char nameFilter[64] = "";
    float maxRef = -1;
    float maxKeys = -1;
    float minRef = -1;
    float minKeys = -1;

    int maxHistory = -1;
    int maxHours = -1;

    bool untradable = false;
    bool noValue = false;
    bool skins = true;

    char apikey[32] = "";

    int lastUpdate = 0;

    int groupSkipPages = 0;
    int groupScanPages = 5;

    int configVersion = 2;

    float timeBetweenRequests = 3;
    float requestTimeout = 6;

    std::optional<JSON::Schema::Response> itemSchema;
    std::optional<JSON::BptfPrices::BptfPrices> itemPrices;
    std::optional<JSON::TfSkins::TfSkins> skinsData;
    std::optional<JSON::TFEffects::TFEffects> itemEffects;
    std::optional<JSON::TFMarket::TFMarket> marketPrices;

    void fetchRequirements();

    void init();

    void save();

    static std::vector<int> parseVersion(std::string version);

    void checkForUpdates();

    float getKeyPrice();

    float getMinPriceInKeys();
    float getMaxPriceInKeys();

    Config() {}

   private:
    static void consoleLog(std::string message, SEVERITY severity = SEVERITY::INFO);
};

#endif  // BPSCANNER_CONFIG_HPP
