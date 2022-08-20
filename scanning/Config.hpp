#ifndef BPSCANNER_CONFIG_HPP
#define BPSCANNER_CONFIG_HPP

#include "../components/Console.hpp"
#include "../json_schemas/Effects.hpp"
#include "../json_schemas/Prices.hpp"
#include "../json_schemas/Schema.hpp"
#include "../json_schemas/Skins.hpp"
#include "../json_schemas/MarketPrices.hpp"
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

    std::vector<JsonSchema::Item> itemSchema = {};
    JsonPrices::Pricelist itemPrices;
    JsonSkins::Skins skinsData;
    JsonEffects::Effects itemEffects;
    std::vector<MarketPricesJson::MarketPrice> marketPrices;

    void fetchRequirements();

    void init();

    void save();

    static std::vector<int> parseVersion(std::string version);

    void checkForUpdates();

    float getKeyPrice();

    float getMinPriceInKeys();
    float getMaxPriceInKeys();

   private:
    static void consoleLog(std::string message, SEVERITY severity = SEVERITY::INFO);
};

#endif  // BPSCANNER_CONFIG_HPP
