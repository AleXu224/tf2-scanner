#include "Scanner.hpp"

#include "../globals.hpp"
#include "../json_schemas/PlayerData.hpp"
#include "Player.hpp"
#include "cpr/cpr.h"
#include "sstream"
#include "string"
#include "vector"

#define consoleLog GLOBALS::console.addOutput

std::vector<std::string> getPlayerIds();

void Scanner::Scan() {
    // consoleLog("Scanning for players...");
    if (isScanning) return;
    if (scanInput.empty()) {
        showDrawer = true;
        return;
    }
    isScanning = true;

    // std::vector<std::string> playerids = {"76561198077264681", "76561198082497772", "76561198143252042", "76561199113821000", "76561198164651434", "76561198095022965", "76561198974533953", "76561198372127799", "76561198068064085"};

    // for (auto &id : playerids) {
    //     // placeholder scan
    //     JsonPlayer::PlayerData playerData;
    //     std::stringstream ss;
    //     ss << "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=" << GLOBALS::scanner.config.apikey << "&format=json&steamids=" << id;
    //     cpr::Response response = cpr::Get(cpr::Url{ss.str()});
    //     if (response.status_code != 200) {
    //         consoleLog("Couldn't get summaries, code(" + std::to_string(response.status_code) + ")", SEVERITY::ERR);
    //         isScanning = false;
    //         return;
    //     }

    //     playerData = nlohmann::json::parse(response.text);

    //     Player player(playerData.response.players[0]);
    //     player.inventory.GetInventory();
    //     playerPushList.push_back(player);
    // }


    std::vector<std::string> playersIds = getPlayerIds();
    if (playersIds.empty()) {
        consoleLog("No players found", SEVERITY::ERR);
        isScanning = false;
        return;
    }

    std::string &status = GLOBALS::scanner.scanStatus;

    for (int i = 0; i < playersIds.size(); i += 100) {
        if (stopScanning) break;
        std::vector<std::string> chunk(playersIds.begin() + i, playersIds.begin() + min(i + 100, playersIds.size()));
        
        status = "Getting summaries " + std::to_string(i / 100) + "/" + std::to_string(playersIds.size() / 100);

        std::stringstream idsString;
        for (auto &id : chunk) {
            idsString << id << ",";
        }

        std::stringstream url;
        url << "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=" << GLOBALS::scanner.config.apikey << "&format=json&steamids=" << idsString.str();

        cpr::Response response = cpr::Get(cpr::Url{url.str()});
        if (response.status_code != 200) {
            consoleLog("Couldn't get summaries, code(" + std::to_string(response.status_code) + ")", SEVERITY::ERR);
            isScanning = false;
            return;
        }

        JsonPlayer::PlayerData playerData = nlohmann::json::parse(response.text);

        for (int j = 0; j < playerData.response.players.size(); j++) {
            if (stopScanning) break;
            status = "Scanning player " + std::to_string(i + j) + "/" + std::to_string(playersIds.size());
            Player player(playerData.response.players[j]);
            if (player.visibility != 3) {
                consoleLog("Player " + player.steamid + " is not public, skipping", SEVERITY::INFO);
                continue;
            }
            player.inventory.GetInventory();
            if (player.inventory.items.empty()) continue;

            #define maxPrice GLOBALS::scanner.config.getMaxPriceInKeys()
            if (maxPrice != -1 && player.inventory.getCurrencyInInventory() < maxPrice) continue;




            playerPushList.push_back(player);
        }
    }



    isScanning = false;
    stopScanning = false;
}

std::string id3ToId64(std::string id3) {
    id3.erase(0, 5);
    id3.erase(id3.size() - 1, 1);
    int id = std::stoi(id3);
    return "7656119" + std::to_string(id + 7960265728);
}

std::vector<std::string> getPlayerIds() {
    ScanType &scanType = GLOBALS::scanner.scanType;
    std::string &input = GLOBALS::scanner.scanInput;
    std::vector<std::string> playersIds;

    if (scanType == ScanType::Steamids) {
        std::regex id64("7656119[0-9]{10}");
        std::regex id3("\\[U:1:[0-9]*\\]");

        std::regex_iterator<std::string::iterator> r64(input.begin(), input.end(), id64);
        std::regex_iterator<std::string::iterator> r3(input.begin(), input.end(), id3);
        std::regex_iterator<std::string::iterator> rend;
        for (std::regex_iterator<std::string::iterator> i = r64; i != rend; ++i) {
            playersIds.push_back(i->str());
        }
        for (std::regex_iterator<std::string::iterator> i = r3; i != rend; ++i) {
            playersIds.push_back(id3ToId64(i->str()));
        }
    } else if (scanType == ScanType::Group) {
        int totalPages = 1;
        int currentPage = 1;
        std::string &scanStatus = GLOBALS::scanner.scanStatus;
        scanStatus = "Getting group members...";
        while (currentPage <= totalPages) {
            std::stringstream groupLink;
            groupLink << input << "/memberslistxml/?p=" << currentPage;

            cpr::Response response = cpr::Get(cpr::Url{groupLink.str()});

            if (response.status_code != 200) {
                consoleLog("Couldn't get group members, code(" + std::to_string(response.status_code) + ")", SEVERITY::ERR);
                return playersIds;
            }

            std::string responseText = response.text;
            std::string totalPagesString = responseText.substr(responseText.find("totalPages") + 11, responseText.find("totalPages") + 15);
            totalPages = std::stoi(totalPagesString);
            totalPages = min(totalPages, 5);
            if (totalPages == 0) {
                consoleLog("Group has no members", SEVERITY::ERR);
                return playersIds;
            }

            std::regex id64("7656119[0-9]{10}");
            std::regex_iterator<std::string::iterator> r64(responseText.begin(), responseText.end(), id64);
            std::regex_iterator<std::string::iterator> rend;
            for (std::regex_iterator<std::string::iterator> i = r64; i != rend; ++i) {
                playersIds.push_back(i->str());
            }
            scanStatus = "Getting group members... (" + std::to_string(currentPage * 1000) + "/" + std::to_string(totalPages * 1000) + ")";
            ++currentPage;
        }

    }

    // for (auto &id : playersIds) {
    //     consoleLog("Found player: " + id, SEVERITY::INFO);
    // }

    return playersIds;
}


// This should be executed in the main thread so that the vector isn't resized while iterating
void Scanner::pushPlayers() {
    if (playerPushList.empty()) return;

    for (auto &player : playerPushList) {
        playerList.push_back(player);
    }

    playerPushList.clear();
}
