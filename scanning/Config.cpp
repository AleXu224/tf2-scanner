#include "Config.hpp"

#include "../globals.hpp"
#include "boost/regex.hpp"
#include "nlohmann/json.hpp"
#include "utility"
#include "vector"
#include "../json_schemas/GithubVersion.hpp"
#include "../components/LoadingScreen.hpp"
#include "../components/Overlay.hpp"
#include "../components/ApiKeyPrompt.hpp"
#include "../components/InfoCard.hpp"
#include "chrono"

void Config::consoleLog(std::string message, SEVERITY severity) {
    GLOBALS::console.addOutput(std::move(message), severity);
}

void Config::fetchRequirements() {
    consoleLog("Fetching requirements...");
    auto *loadingScreen = new LoadingScreen();
    Overlay::addOverlay(loadingScreen);

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

    // Last update check
    using namespace std::chrono;
    int currentTime = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    const int updateInterval = 60 * 60 * 24 * 7; // 1 week
    bool shouldUpdate = (currentTime - lastUpdate) > updateInterval;
    if (shouldUpdate) consoleLog("Last update was more than a week ago, updating...");

    // Item schema
    std::ifstream schemaFile;
    schemaFile.open(storagePath + "schema.json");
    if (schemaFile.is_open() && !shouldUpdate) {
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
                Overlay::addOverlay(new InfoCard("Failed to fetch schema, aborting"));
                return;
            }
            JsonSchema::Schema schema = nlohmann::json::parse(schemaResponse.text);

            itemSchema.insert(itemSchema.end(), schema.result.items.begin(), schema.result.items.end());

            if (schema.result.next == nullptr) break;
            start = *schema.result.next;
            consoleLog("Next: " + std::to_string(start));
        }

        std::ofstream schemaFileOut;
        schemaFileOut.open(storagePath + "schema.json", std::ofstream::out | std::ofstream::trunc);
        nlohmann::json itemSchemaJson = itemSchema;
        schemaFileOut << itemSchemaJson.dump();
        schemaFileOut.close();

        consoleLog("Schema file saved");
    }

    // Item prices
    std::ifstream pricesFile;
    pricesFile.open(storagePath + "prices.json");
    if (pricesFile.is_open() && !shouldUpdate) {
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
            Overlay::addOverlay(new InfoCard("Failed to fetch prices, aborting"));
            return;
        }
        JsonPrices::Pricelist prices = nlohmann::json::parse(pricesResponse.text);

        itemPrices = prices;

        std::ofstream pricesFileOut;
        pricesFileOut.open(storagePath + "prices.json", std::ofstream::out | std::ofstream::trunc);
        nlohmann::json itemPricesJson = itemPrices;
        pricesFileOut << itemPricesJson.dump();
        pricesFileOut.close();

        consoleLog("Prices file saved");
    }

    // Skins
    std::ifstream skinsFile;
    skinsFile.open(storagePath + "skins.json");
    if (skinsFile.is_open() && !shouldUpdate) {
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
            Overlay::addOverlay(new InfoCard("Failed to fetch skins, aborting"));
            return;
        }
        JsonSkins::Skins skins = nlohmann::json::parse(skinsResponse.text);

        skinsData = skins;

        std::ofstream skinsFileOut;
        skinsFileOut.open(storagePath + "skins.json", std::ofstream::out | std::ofstream::trunc);
        nlohmann::json skinsJson = skinsData;
        skinsFileOut << skinsJson.dump();
        skinsFileOut.close();

        consoleLog("Skins file saved");
    }

    // item effects
    std::ifstream effectsFile;
    effectsFile.open(storagePath + "effects.json");
    if (effectsFile.is_open() && !shouldUpdate) {
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
            Overlay::addOverlay(new InfoCard("Failed to fetch effects, aborting"));
            return;
        }
        JsonEffects::Effects effects = nlohmann::json::parse(effectsResponse.text);

        itemEffects = effects;

        std::ofstream effectsFileOut;
        effectsFileOut.open(storagePath + "effects.json", std::ofstream::out | std::ofstream::trunc);
        nlohmann::json effectsJson = itemEffects;
        effectsFileOut << effectsJson.dump();
        effectsFileOut.close();

        consoleLog("Effects file saved");
    }

    // market prices
    std::ifstream marketFile;
    marketFile.open(storagePath + "market.json");
    if (marketFile.is_open() && !shouldUpdate) {
        consoleLog("Market file found, loading...");
        std::stringstream buffer;
        buffer << marketFile.rdbuf();
        marketFile.close();

        marketPrices = nlohmann::json::parse(buffer.str());
    } else {
        consoleLog("Market file does not exist, fetching...", SEVERITY::INFO);
        std::string url = "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/marketPrices.json";
        auto marketResponse = cpr::Get(cpr::Url{url});

        consoleLog("Market response code: " + std::to_string(marketResponse.status_code));
        if (marketResponse.status_code != 200) {
            consoleLog("Failed to fetch market, aborting", SEVERITY::ERR);
            return;
        }
        std::vector<MarketPricesJson::MarketPrice> market = nlohmann::json::parse(marketResponse.text);

        marketPrices = market;

        std::ofstream marketFileOut;
        marketFileOut.open(storagePath + "market.json", std::ofstream::out | std::ofstream::trunc);
        nlohmann::json marketJson = marketPrices;
        marketFileOut << marketJson.dump();
        marketFileOut.close();

        consoleLog("Market file saved");
    }

    consoleLog("Loading complete");
    if (shouldUpdate) {
        lastUpdate = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    }
    save();
    loadingScreen->shouldClose = true;
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
        // Null check so that users coming from an older version don't crash
        if (!config["lastUpdate"].is_null()) lastUpdate = config["lastUpdate"].get<int>();
        if (!config["groupSkipPages"].is_null()) groupSkipPages = config["groupSkipPages"].get<int>();
        if (!config["groupScanPages"].is_null()) groupScanPages = config["groupScanPages"].get<int>();
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
        config["lastUpdate"] = lastUpdate;
        config["groupSkipPages"] = groupSkipPages;
        config["groupScanPages"] = groupScanPages;

        std::ofstream configFileOut;
        configFileOut.open(storagePath + "config.json");
        configFileOut << config.dump();
        configFileOut.close();
    }

    if (strlen(apikey) < 32) {
        consoleLog("API key is invalid, opening prompt..", SEVERITY::WARNING);
        Overlay::addOverlay(new ApiKeyPrompt());
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
    config["lastUpdate"] = lastUpdate;
    config["groupSkipPages"] = groupSkipPages;
    config["groupScanPages"] = groupScanPages;

    std::ofstream configFileOut;
    configFileOut.open(storagePath + "config.json");
    configFileOut << config.dump();
    configFileOut.close();
}

std::vector<int> Config::parseVersion(std::string version) {
    boost::regex versionRegex("[0-9]+");
    std::vector<int> versionNumbers(3);

    boost::smatch versionMatch;
    boost::regex_search(version, versionMatch, versionRegex);
    versionNumbers[0] = std::stoi(versionMatch[0]);
    version = versionMatch.suffix();
    boost::regex_search(version, versionMatch, versionRegex);
    versionNumbers[1] = std::stoi(versionMatch[0]);
    version = versionMatch.suffix();
    boost::regex_search(version, versionMatch, versionRegex);
    versionNumbers[2] = std::stoi(versionMatch[0]);

    return versionNumbers;
}

void Config::checkForUpdates() {
    auto currentVersion = parseVersion(SCANNER_VERSION);

    auto updateResponse = cpr::Get(cpr::Url{"https://api.github.com/repos/AleXu224/tf2-scanner/releases/latest"});

    if (updateResponse.status_code != 200) {
        consoleLog("Failed to check for updates", SEVERITY::WARNING);
        Overlay::addOverlay(new InfoCard("Failed checking for updates"));
        return;
    }

    JsonGithubVersion::GithubVersion githubVersion = nlohmann::json::parse(updateResponse.text);
    auto githubVersionNumbers = parseVersion(githubVersion.tag_name);

    for (int i = 0; i < 3; i++) {
        if (githubVersionNumbers[i] > currentVersion[i]) {
            consoleLog("New version available: " + githubVersion.tag_name, SEVERITY::INFO);
            GLOBALS::scanner.updateAvailable = true;
            GLOBALS::scanner.updateVersion = githubVersion.tag_name;
            GLOBALS::scanner.updateLink = githubVersion.html_url;
            return;
        }
        if (githubVersionNumbers[i] < currentVersion[i]) {
            consoleLog("You are currently running a version that is ahead of master", SEVERITY::WARNING);
            return;
        }
    }
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