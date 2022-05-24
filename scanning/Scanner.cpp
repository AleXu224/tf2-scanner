#include "Scanner.hpp"

#include "../globals.hpp"
#include "../json_schemas/PlayerData.hpp"
#include "Player.hpp"
#include "cpr/cpr.h"
#include "sstream"
#include "string"
#include "vector"

#define consoleLog GLOBALS::console.addOutput

void Scanner::Scan() {
    if (isScanning) return;
    isScanning = true;

    std::vector<std::string> playerids = {"76561198077264681", "76561198082497772", "76561198143252042", "76561199113821000", "76561198164651434", "76561198095022965", "76561198974533953", "76561198372127799", "76561198068064085"};

    for (auto &id : playerids) {
        // placeholder scan
        JsonPlayer::PlayerData playerData;
        std::stringstream ss;
        ss << "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=" << GLOBALS::scanner.config.apikey << "&format=json&steamids=" << id;
        cpr::Response response = cpr::Get(cpr::Url{ss.str()});
        if (response.status_code != 200) {
            consoleLog("Couldn't get summaries, code(" + std::to_string(response.status_code) + ")", SEVERITY::ERR);
            isScanning = false;
            return;
        }

        playerData = nlohmann::json::parse(response.text);

        Player player(playerData.response.players[0]);
        player.inventory.GetInventory();
        playerPushList.push_back(player);
    }

    isScanning = false;
}

// This should be executed in the main thread so that the vector isn't resized while iterating
void Scanner::pushPlayers() {
    if (playerPushList.empty()) return;

    for (auto &player : playerPushList) {
        playerList.push_back(player);
    }

    playerPushList.clear();
}
