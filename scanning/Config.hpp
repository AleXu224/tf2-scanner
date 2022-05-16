//
// Created by Squizell on 15/05/2022.
//

#ifndef BPSCANNER_CONFIG_HPP
#define BPSCANNER_CONFIG_HPP

#include "iostream"
#include "../components/Console.hpp"
#include "filesystem"
#include "cpr/cpr.h"
#include "regex"

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


    void fetchRequirements() {
        consoleLog("Fetching requirements...");

        std::string version = "v1.2.3";
        // get version numbers from string

        std::regex versionRegex("[0-9]+");
        std::smatch versionMatch;
        std::regex_search(version, versionMatch, versionRegex);
        consoleLog("Version: " + std::string(versionMatch[0].str()));



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

        using namespace cpr;


    }

private:
    void consoleLog(std::string message, int severity = 0);
};


#endif //BPSCANNER_CONFIG_HPP
