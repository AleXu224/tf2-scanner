#include "Scanner.hpp"
#include "cpr/cpr.h"
#include "Player.hpp"
#include "../json_schemas/PlayerData.hpp"
#include "../globals.hpp"
#include "string"
#include "sstream"
#include "vector"

#define consoleLog GLOBALS::console.addOutput

void Scanner::Scan() {
    if (isScanning) return;
    isScanning = true;

    // placeholder scan
    JsonPlayer::PlayerData playerData;
    std::stringstream ss;
    ss << "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=" << GLOBALS::scanner.config.apikey << "&format=json&steamids=76561198177485682";
    cpr::Response response = cpr::Get(cpr::Url{ss.str()});
    if (response.status_code != 200) {
        consoleLog("Couldn't get summaries, code(" + std::to_string(response.status_code) + ")", SEVERITY::ERR);
        isScanning = false;
        return;
    }

    playerData = nlohmann::json::parse(response.text);

    Player player(playerData.response.players[0]);
    player.inventory.GetInventory();
    playerList.push_back(player);
    GLOBALS::console.addOutput(player);

    isScanning = false;
}
