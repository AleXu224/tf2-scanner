#include "Config.hpp"

#include "../globals.hpp"
#include "nlohmann/json.hpp"
#include "utility"
#include "vector"

void Config::consoleLog(std::string message, SEVERITY severity) {
    GLOBALS::console.addOutput(std::move(message), severity);
}

void Config::fetchRequirements() {
    consoleLog("Fetching requirements...");
    GLOBALS::scanner.showLoadingScreen = true;

    if (strlen(apikey) < 32) {
        consoleLog("API key is empty, can't fetch requirements", SEVERITY::ERR);
        return;
    }

    std::string localAppData = std::getenv("LOCALAPPDATA");
    std::string storagePath = localAppData + "\\BPScanner\\";
    consoleLog("Storage Path: " + storagePath);

    if (!std::filesystem::exists(storagePath)) {
        consoleLog("Storage folder does not exist, creating...");
        std::filesystem::create_directory(storagePath);
    }

    // Item schema
    std::ifstream schemaFile;
    schemaFile.open(storagePath + "schema.json");
    if (schemaFile.is_open()) {
        consoleLog("Schema file found, loading...");
        std::stringstream buffer;
        buffer << schemaFile.rdbuf();
        schemaFile.close();

        itemSchema = nlohmann::json::parse(buffer.str());
    } else {
        consoleLog("Schema file does not exist, fetching...", SEVERITY::INFO);
        std::stringstream url;
        int start = 0;
        while (true) {
            url.str(std::string());
            url << "https://api.steampowered.com/IEconItems_440/GetSchemaItems/v0001/?key=" << apikey
                << "&language=en_US&start=" << start;
            auto schemaResponse = cpr::Get(cpr::Url{url.str()});

            consoleLog("Schema response code: " + std::to_string(schemaResponse.status_code));
            if (schemaResponse.status_code != 200) {
                consoleLog("Failed to fetch schema, aborting", SEVERITY::ERR);
                return;
            }
            JsonSchema::Schema schema = nlohmann::json::parse(schemaResponse.text);

            itemSchema.insert(itemSchema.end(), schema.result.items.begin(), schema.result.items.end());

            if (schema.result.next == nullptr) break;
            start = *schema.result.next;
            consoleLog("Next: " + std::to_string(start));
        }

        std::ofstream schemaFileOut;
        schemaFileOut.open(storagePath + "schema.json");
        nlohmann::json itemSchemaJson = itemSchema;
        schemaFileOut << itemSchemaJson.dump();
        schemaFileOut.close();

        consoleLog("Schema file saved");
    }

    // Item prices
    std::ifstream pricesFile;
    pricesFile.open(storagePath + "prices.json");
    if (pricesFile.is_open()) {
        consoleLog("Prices file found, loading...");
        std::stringstream buffer;
        buffer << pricesFile.rdbuf();
        pricesFile.close();

        itemPrices = nlohmann::json::parse(buffer.str());
    } else {
        consoleLog("Prices file does not exist, fetching...", SEVERITY::INFO);
        std::string url = "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/schema_bptf.json";
        auto pricesResponse = cpr::Get(cpr::Url{url});

        consoleLog("Prices response code: " + std::to_string(pricesResponse.status_code));
        if (pricesResponse.status_code != 200) {
            consoleLog("Failed to fetch prices, aborting", SEVERITY::ERR);
            return;
        }
        JsonPrices::Pricelist prices = nlohmann::json::parse(pricesResponse.text);

        itemPrices = prices;

        std::ofstream pricesFileOut;
        pricesFileOut.open(storagePath + "prices.json");
        nlohmann::json itemPricesJson = itemPrices;
        pricesFileOut << itemPricesJson.dump();
        pricesFileOut.close();

        consoleLog("Prices file saved");
    }

    // Skins
    std::ifstream skinsFile;
    skinsFile.open(storagePath + "skins.json");
    if (skinsFile.is_open()) {
        consoleLog("Skins file found, loading...");
        std::stringstream buffer;
        buffer << skinsFile.rdbuf();
        skinsFile.close();

        skinsData = nlohmann::json::parse(buffer.str());
    } else {
        consoleLog("Skins file does not exist, fetching...", SEVERITY::INFO);
        std::string url = "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/skins.json";
        auto skinsResponse = cpr::Get(cpr::Url{url});

        consoleLog("Skins response code: " + std::to_string(skinsResponse.status_code));
        if (skinsResponse.status_code != 200) {
            consoleLog("Failed to fetch skins, aborting", SEVERITY::ERR);
            return;
        }
        JsonSkins::Skins skins = nlohmann::json::parse(skinsResponse.text);

        skinsData = skins;

        std::ofstream skinsFileOut;
        skinsFileOut.open(storagePath + "skins.json");
        nlohmann::json skinsJson = skinsData;
        skinsFileOut << skinsJson.dump();
        skinsFileOut.close();

        consoleLog("Skins file saved");
    }

    // item effects
    std::ifstream effectsFile;
    effectsFile.open(storagePath + "effects.json");
    if (effectsFile.is_open()) {
        consoleLog("Effects file found, loading...");
        std::stringstream buffer;
        buffer << effectsFile.rdbuf();
        effectsFile.close();

        itemEffects = nlohmann::json::parse(buffer.str());
    } else {
        consoleLog("Effects file does not exist, fetching...", SEVERITY::INFO);
        std::string url = "https://raw.githubusercontent.com/mninc/tf2-effects/master/effects.json";
        auto effectsResponse = cpr::Get(cpr::Url{url});

        consoleLog("Effects response code: " + std::to_string(effectsResponse.status_code));
        if (effectsResponse.status_code != 200) {
            consoleLog("Failed to fetch effects, aborting", SEVERITY::ERR);
            return;
        }
        JsonEffects::Effects effects = nlohmann::json::parse(effectsResponse.text);

        itemEffects = effects;

        std::ofstream effectsFileOut;
        effectsFileOut.open(storagePath + "effects.json");
        nlohmann::json effectsJson = itemEffects;
        effectsFileOut << effectsJson.dump();
        effectsFileOut.close();

        consoleLog("Effects file saved");
    }

    consoleLog("Loading complete");
    GLOBALS::scanner.showLoadingScreen = false;
}

void Config::init() {
    consoleLog("Initializing config...");

    std::string localAppData = std::getenv("LOCALAPPDATA");
    std::string storagePath = localAppData + "\\BPScanner\\";
    consoleLog("Storage Path: " + storagePath);

    if (!std::filesystem::exists(storagePath)) {
        consoleLog("Storage folder does not exist, creating...");
        std::filesystem::create_directory(storagePath);
    }

    // Load config
    std::ifstream configFile;
    configFile.open(storagePath + "config.json");
    if (configFile.is_open()) {
        consoleLog("Config file found, loading...");
        std::stringstream buffer;
        buffer << configFile.rdbuf();
        configFile.close();

        auto config = nlohmann::json::parse(buffer.str());
        maxRef = config["maxRef"].get<float>();
        maxKeys = config["maxKeys"].get<float>();
        minRef = config["minRef"].get<float>();
        minKeys = config["minKeys"].get<float>();
        maxHistory = config["maxHistory"].get<int>();
        maxHours = config["maxHours"].get<int>();
        untradable = config["untradable"].get<bool>();
        noValue = config["noValue"].get<bool>();
        skins = config["skins"].get<bool>();
        strcpy(apikey, config["apikey"].get<std::string>().c_str());
    } else {
        consoleLog("Config file does not exist, creating...", SEVERITY::INFO);
        nlohmann::json config;
        config["maxRef"] = maxRef;
        config["maxKeys"] = maxKeys;
        config["minRef"] = minRef;
        config["minKeys"] = minKeys;
        config["minKeys"] = minKeys;
        config["maxHistory"] = maxHistory;
        config["maxHours"] = maxHours;
        config["untradable"] = untradable;
        config["noValue"] = noValue;
        config["skins"] = skins;
        config["apikey"] = apikey;

        std::ofstream configFileOut;
        configFileOut.open(storagePath + "config.json");
        configFileOut << config.dump();
        configFileOut.close();
    }

    if (strlen(apikey) < 32) {
        consoleLog("API key is invalid, opening prompt..", SEVERITY::WARNING);
        GLOBALS::scanner.showApiKeyPrompt = true;
    } else {
        fetchRequirements();
    }
}

void Config::save() {
    consoleLog("Saving config...");

    std::string localAppData = std::getenv("LOCALAPPDATA");
    std::string storagePath = localAppData + "\\BPScanner\\";
    consoleLog("Storage Path: " + storagePath);

    if (!std::filesystem::exists(storagePath)) {
        consoleLog("Storage folder does not exist, creating...");
        std::filesystem::create_directory(storagePath);
    }

    nlohmann::json config;
    config["maxRef"] = maxRef;
    config["maxKeys"] = maxKeys;
    config["minRef"] = minRef;
    config["minKeys"] = minKeys;
    config["minKeys"] = minKeys;
    config["maxHistory"] = maxHistory;
    config["maxHours"] = maxHours;
    config["untradable"] = untradable;
    config["noValue"] = noValue;
    config["skins"] = skins;
    config["apikey"] = apikey;

    std::ofstream configFileOut;
    configFileOut.open(storagePath + "config.json");
    configFileOut << config.dump();
    configFileOut.close();
}

std::vector<int> Config::parseVersion(std::string version) {
    std::regex versionRegex("[0-9]+");
    std::vector<int> versionNumbers(3);

    std::smatch versionMatch;
    std::regex_search(version, versionMatch, versionRegex);
    versionNumbers[0] = std::stoi(versionMatch[0]);
    version = versionMatch.suffix();
    std::regex_search(version, versionMatch, versionRegex);
    versionNumbers[1] = std::stoi(versionMatch[0]);
    version = versionMatch.suffix();
    std::regex_search(version, versionMatch, versionRegex);
    versionNumbers[2] = std::stoi(versionMatch[0]);

    return versionNumbers;
}

float Config::getKeyPrice() {
#define PRICES GLOBALS::scanner.config.itemPrices.response.items
    auto keyPriceCraftable = PRICES.at(CURRENCY_KEY).prices.at("6").tradable.craftable;
    auto keyPriceVector = std::get<std::vector<JsonPrices::CraftableElement>>(*keyPriceCraftable);
    auto keyPrice = *keyPriceVector.at(0).value;
    return keyPrice;
}

float Config::getMinPriceInKeys() {
    return max(minKeys, 0) + max(minRef / getKeyPrice(), 0);
}

float Config::getMaxPriceInKeys() {
    if (maxKeys == -1 && maxRef == -1) return -1;
    return max(maxKeys, 0) + max(maxRef / getKeyPrice(), 0);
}