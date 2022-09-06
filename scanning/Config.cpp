#define _CRT_SECURE_NO_WARNINGS
#include "Config.hpp"
#include "../globals.hpp"
#include "boost/regex.hpp"
#include "yyjson.h"
#include "utility"
#include "vector"
#include "../json_schemas/GithubReleases.hpp"
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
        std::string schemaJson = buffer.str();

        itemSchema = JSON::Schema::fromJson(schemaJson);
    } else {
        consoleLog("Schema file does not exist, fetching...", SEVERITY::INFO);
        std::stringstream url;
        int start = 0;
        bool schemaCreated = false;
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
            // JsonSchema::Schema schema = nlohmann::json::parse(schemaResponse.text);
            JSON::Schema::Response schema = JSON::Schema::fromJson(schemaResponse.text);
            if (!schemaCreated) {
                itemSchema = schema;
                schemaCreated = true;
            } else {
                itemSchema->result.items.insert(itemSchema->result.items.end(), schema.result.items.begin(),
                                                schema.result.items.end());
            }

            if (!schema.result.next.has_value()) break;
            start = *schema.result.next;
            consoleLog("Next: " + std::to_string(start));
        }

        std::ofstream schemaFileOut;
        schemaFileOut.open(storagePath + "schema.json", std::ofstream::out | std::ofstream::trunc);
        schemaFileOut << JSON::Schema::toJson(itemSchema.value());
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
        std::string pricesJson = buffer.str();
        itemPrices = JSON::BptfPrices::fromJson(pricesJson);
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
        itemPrices = JSON::BptfPrices::fromJson(pricesResponse.text);

        std::ofstream pricesFileOut;
        pricesFileOut.open(storagePath + "prices.json", std::ofstream::out | std::ofstream::trunc);
        pricesFileOut << JSON::BptfPrices::toJson(itemPrices.value());
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
        std::string skinsJson = buffer.str();
        skinsData = JSON::TfSkins::fromJson(skinsJson);
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

        skinsData = JSON::TfSkins::fromJson(skinsResponse.text);

        std::ofstream skinsFileOut;
        skinsFileOut.open(storagePath + "skins.json", std::ofstream::out | std::ofstream::trunc);
        skinsFileOut << JSON::TfSkins::toJson(skinsData.value());
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
        std::string effectsJson = buffer.str();
        itemEffects = JSON::TFEffects::fromJson(effectsJson);
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

        itemEffects = JSON::TFEffects::fromJson(effectsResponse.text);

        std::ofstream effectsFileOut;
        effectsFileOut.open(storagePath + "effects.json", std::ofstream::out | std::ofstream::trunc);
        effectsFileOut << JSON::TFEffects::toJson(itemEffects.value());
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
        std::string marketJson = buffer.str();
        marketPrices = JSON::TFMarket::fromJson(marketJson);
    } else {
        consoleLog("Market file does not exist, fetching...", SEVERITY::INFO);
        std::string url = "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/marketPrices.json";
        auto marketResponse = cpr::Get(cpr::Url{url});

        consoleLog("Market response code: " + std::to_string(marketResponse.status_code));
        if (marketResponse.status_code != 200) {
            consoleLog("Failed to fetch market, aborting", SEVERITY::ERR);
            return;
        }

        marketPrices = JSON::TFMarket::fromJson(marketResponse.text);

        std::ofstream marketFileOut;
        marketFileOut.open(storagePath + "market.json", std::ofstream::out | std::ofstream::trunc);
        marketFileOut << JSON::TFMarket::toJson(marketPrices.value());
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
        std::string configJson = buffer.str();

        auto *doc = yyjson_read(configJson.data(), configJson.size(), 0);
        auto *root = yyjson_doc_get_root(doc);

        maxRef = yyjson_get_real(yyjson_obj_get(root, "maxRef"));
        maxKeys = yyjson_get_real(yyjson_obj_get(root, "maxKeys"));
        minRef = yyjson_get_real(yyjson_obj_get(root, "minRef"));
        minKeys = yyjson_get_real(yyjson_obj_get(root, "minKeys"));
        maxHistory = yyjson_get_int(yyjson_obj_get(root, "maxHistory"));
        maxHours = yyjson_get_int(yyjson_obj_get(root, "maxHours"));
        untradable = yyjson_get_bool(yyjson_obj_get(root, "untradable"));
        noValue = yyjson_get_bool(yyjson_obj_get(root, "noValue"));
        skins = yyjson_get_bool(yyjson_obj_get(root, "skins"));

        // Null check so that users coming from an older version don't crash
        auto lastUpdateObj = yyjson_obj_get(root, "lastUpdate");
        if (lastUpdateObj && !yyjson_is_null(lastUpdateObj)) lastUpdate = yyjson_get_int(lastUpdateObj);

        auto groupSkipPagesObj = yyjson_obj_get(root, "groupSkipPages");
        if (groupSkipPagesObj && !yyjson_is_null(groupSkipPagesObj)) groupSkipPages = yyjson_get_int(groupSkipPagesObj);

        auto groupScanPagesObj = yyjson_obj_get(root, "groupScanPages");
        if (groupScanPagesObj && !yyjson_is_null(groupScanPagesObj)) groupScanPages = yyjson_get_int(groupScanPagesObj);

        auto apikeyObj = yyjson_obj_get(root, "apikey");
        if (apikeyObj && !yyjson_is_null(apikeyObj)) strcpy(apikey, yyjson_get_str(apikeyObj));

        auto timeBetweenRequestsObj = yyjson_obj_get(root, "timeBetweenRequests");
        if (timeBetweenRequestsObj && !yyjson_is_null(timeBetweenRequestsObj)) timeBetweenRequests = yyjson_get_real(timeBetweenRequestsObj);

        auto requestTimeoutObj = yyjson_obj_get(root, "requestTimeout");
        if (requestTimeoutObj && !yyjson_is_null(requestTimeoutObj)) requestTimeout = yyjson_get_real(requestTimeoutObj);

        auto configVersionObj = yyjson_obj_get(root, "configVersion");
        if (configVersionObj && !yyjson_is_null(configVersionObj)) {
            configVersion = yyjson_get_int(configVersionObj);
        } else {
            lastUpdate = 0; // In version 2 the item schema format was updated so a force update is needed
        }
    } else {
        consoleLog("Config file does not exist, creating...", SEVERITY::INFO);
        auto *doc = yyjson_mut_doc_new(nullptr);
        auto *root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);
        yyjson_mut_obj_add_real(doc, root, "maxRef", maxRef);
        yyjson_mut_obj_add_real(doc, root, "maxKeys", maxKeys);
        yyjson_mut_obj_add_real(doc, root, "minRef", minRef);
        yyjson_mut_obj_add_real(doc, root, "minKeys", minKeys);
        yyjson_mut_obj_add_int(doc, root, "maxHistory", maxHistory);
        yyjson_mut_obj_add_int(doc, root, "maxHours", maxHours);
        yyjson_mut_obj_add_bool(doc, root, "untradable", untradable);
        yyjson_mut_obj_add_bool(doc, root, "noValue", noValue);
        yyjson_mut_obj_add_bool(doc, root, "skins", skins);
        yyjson_mut_obj_add_str(doc, root, "apikey", apikey);
        yyjson_mut_obj_add_int(doc, root, "lastUpdate", lastUpdate);
        yyjson_mut_obj_add_int(doc, root, "groupSkipPages", groupSkipPages);
        yyjson_mut_obj_add_int(doc, root, "groupScanPages", groupScanPages);
        yyjson_mut_obj_add_int(doc, root, "configVersion", configVersion);
        yyjson_mut_obj_add_real(doc, root, "timeBetweenRequests", timeBetweenRequests);
        yyjson_mut_obj_add_real(doc, root, "requestTimeout", requestTimeout);

        std::string configJson = yyjson_mut_write(doc, 0, nullptr);
        yyjson_mut_doc_free(doc);


        std::ofstream configFileOut;
        configFileOut.open(storagePath + "config.json");
        configFileOut << configJson;
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

    auto *doc = yyjson_mut_doc_new(nullptr);
    auto *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_obj_add_real(doc, root, "maxRef", maxRef);
    yyjson_mut_obj_add_real(doc, root, "maxKeys", maxKeys);
    yyjson_mut_obj_add_real(doc, root, "minRef", minRef);
    yyjson_mut_obj_add_real(doc, root, "minKeys", minKeys);
    yyjson_mut_obj_add_int(doc, root, "maxHistory", maxHistory);
    yyjson_mut_obj_add_int(doc, root, "maxHours", maxHours);
    yyjson_mut_obj_add_bool(doc, root, "untradable", untradable);
    yyjson_mut_obj_add_bool(doc, root, "noValue", noValue);
    yyjson_mut_obj_add_bool(doc, root, "skins", skins);
    yyjson_mut_obj_add_str(doc, root, "apikey", apikey);
    yyjson_mut_obj_add_int(doc, root, "lastUpdate", lastUpdate);
    yyjson_mut_obj_add_int(doc, root, "groupSkipPages", groupSkipPages);
    yyjson_mut_obj_add_int(doc, root, "groupScanPages", groupScanPages);
    yyjson_mut_obj_add_int(doc, root, "configVersion", configVersion);
    yyjson_mut_obj_add_real(doc, root, "timeBetweenRequests", timeBetweenRequests);
    yyjson_mut_obj_add_real(doc, root, "requestTimeout", requestTimeout);

    std::string configJson = yyjson_mut_write(doc, 0, nullptr);
    yyjson_mut_doc_free(doc);

    std::ofstream configFileOut;
    configFileOut.open(storagePath + "config.json");
    configFileOut << configJson;
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

    JSON::GithubReleases::GithubReleases githubVersion = JSON::GithubReleases::fromJson(updateResponse.text);
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
#define PRICES GLOBALS::scanner.config.itemPrices->response.items
    auto keyPriceCraftable = PRICES.at(CURRENCY_KEY).prices.at("6").tradable.craftable.value();
    auto &keyPriceVector = std::get<std::vector<JSON::BptfPrices::Element>>(keyPriceCraftable);
    auto keyPrice = keyPriceVector.at(0).value.value();
    return keyPrice;
    
}

float Config::getMinPriceInKeys() {
    return max(minKeys, 0) + max(minRef / getKeyPrice(), 0);
}

float Config::getMaxPriceInKeys() {
    if (maxKeys == -1 && maxRef == -1) return -1;
    return max(maxKeys, 0) + max(maxRef / getKeyPrice(), 0);
}