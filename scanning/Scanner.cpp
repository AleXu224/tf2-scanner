#include "Scanner.hpp"

#include "../globals.hpp"
#include "../json_schemas/PlayerData.hpp"
#include "../components/Overlay.hpp"
#include "../components/InfoCard.hpp"
#include "Player.hpp"
#include "cpr/cpr.h"
#include "sstream"
#include "string"
#include "vector"

#define consoleLog GLOBALS::console.addOutput

std::vector<std::string> getPlayerIds();

void Scanner::Scan() {
    if (isScanning) return;
    if (scanInput.empty()) {
        showDrawer = true;
        return;
    }
    GLOBALS::scanner.config.save();
    isScanning = true;

    std::vector<std::string> playersIds = getPlayerIds();
    if (playersIds.empty()) {
        consoleLog("No players found", SEVERITY::ERR);
        Overlay::addOverlay(new InfoCard("Scan input is empty"));
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
            Overlay::addOverlay(new InfoCard("Couldn't get summaries, Steam is probably down"));
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
            if (maxPrice != -1 && player.inventory.getCurrencyInInventory() > maxPrice) continue;

            std::thread miscThread([player]() mutable -> Player {
                player.getHours();
                player.getLevel();
                player.getHistories();
                if (GLOBALS::scanner.config.maxHistory != -1 && (player.histories > GLOBALS::scanner.config.maxHistory || player.histories == -1)) {
                    consoleLog("Player " + player.steamid + " has too many histories, skipping", SEVERITY::INFO);
                    return player;
                }
                if (GLOBALS::scanner.config.maxHours != -1 && (player.hours > GLOBALS::scanner.config.maxHours || player.hours == -1)) {
                    consoleLog("Player " + player.steamid + " has too many hours, skipping", SEVERITY::INFO);
                    return player;
                }
                GLOBALS::scanner.playerPushList.push_back(player);
                return player;
            });
            miscThread.detach();
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
        int currentPage = GLOBALS::scanner.config.groupSkipPages + 1;
        std::string &scanStatus = GLOBALS::scanner.scanStatus;
        scanStatus = "Getting group members...";
        do {
            if (GLOBALS::scanner.stopScanning) break;
            consoleLog("Getting group members page " + std::to_string(currentPage) + "/" + std::to_string(totalPages), SEVERITY::INFO);
            std::stringstream groupLink;
            groupLink << input << "/memberslistxml/?p=" << currentPage;

            cpr::Response response = cpr::Get(cpr::Url{groupLink.str()});

            if (response.status_code == 429) {
                consoleLog("Too many requests, stopping", SEVERITY::ERR);
                consoleLog("Group members page only allows 30 requests before banning you for ~15-30 minutes", SEVERITY::ERR);
            }

            if (response.status_code != 200) {
                consoleLog("Couldn't get group members, code(" + std::to_string(response.status_code) + ")", SEVERITY::ERR);
                return playersIds;
            }

            std::string responseText = response.text;
            std::string totalPagesString = responseText.substr(responseText.find("totalPages") + 11, responseText.find("totalPages") + 15);
            totalPages = std::stoi(totalPagesString);
            totalPages = min(totalPages, GLOBALS::scanner.config.groupSkipPages + GLOBALS::scanner.config.groupScanPages);
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
        } while (currentPage <= totalPages);
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

void Scanner::popPlayers() {
    if (playerList.empty()) return;

    // std::sort(playerPopList.begin(), playerPopList.end(), [](int &a , int &b) {
    //     return a < b;
    // });

    for (auto &id : playerPopList) {
        playerList.erase(playerList.begin() + id);
    }

    playerPopList.clear();
}
