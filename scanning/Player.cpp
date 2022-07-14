#include "Player.hpp"
#include "sstream"
#include "../globals.hpp"
#include "cpr/cpr.h"

#define consoleLog GLOBALS::console.addOutput

Player::Player(JsonPlayer::Player &player) : inventory(player.steamid) {
    steamid = player.steamid;
    name = player.personaname;
    avatarUrl = player.avatarmedium;
    visibility = player.communityvisibilitystate;
}

void Player::getHours() {
    std::stringstream ss;
    ss << "https://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=";
    ss << GLOBALS::scanner.config.apikey;
    ss << "&steamid=" << steamid;
    ss << "&format=json&include_appinfo=1&include_played_free_games=1";

    auto r = cpr::Get(cpr::Url{ss.str()});
}

void Player::ToConsole() {
    using namespace ImGui;
    if (TreeNode((void *)(intptr_t)this, "Name %s", name.c_str())) {
        Text("SteamID: %s", steamid.c_str());
        Text("Name: %s", name.c_str());
        Text("Avatar: %s", avatarUrl.c_str());
        Text("Visibility: %d", visibility);
        Text("Level: %d", level);
        Text("Histories: %d", histories);
        Text("Hours: %d", hours);
        inventory.ToConsole();
        TreePop();
    }
}
