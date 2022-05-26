#ifndef BPSCANNER_CONFIG_HPP
#define BPSCANNER_CONFIG_HPP

#include "../components/Console.hpp"
#include "../json_schemas/Effects.hpp"
#include "../json_schemas/Prices.hpp"
#include "../json_schemas/Schema.hpp"
#include "../json_schemas/Skins.hpp"
#include "cpr/cpr.h"
#include "filesystem"
#include "iostream"
#include "regex"
#include "sstream"

class Config {
   public:
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

    std::vector<JsonSchema::Item> itemSchema = {};
    JsonPrices::Pricelist itemPrices;
    JsonSkins::Skins skinsData;
    JsonEffects::Effects itemEffects;

    void fetchRequirements();

    void init();

    void save();

    static std::vector<int> parseVersion(std::string version);

   private:
    static void consoleLog(std::string message, SEVERITY severity = SEVERITY::INFO);
};

#endif  // BPSCANNER_CONFIG_HPP
