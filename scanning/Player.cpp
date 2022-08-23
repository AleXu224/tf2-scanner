#include "Player.hpp"
#include "sstream"
#include "string"
#include "../globals.hpp"
#include "cpr/cpr.h"
#include "../json_schemas/SteamGames.hpp"
#include "../json_schemas/SteamLevel.hpp"
#include "boost/regex.hpp"

#define consoleLog GLOBALS::console.addOutput

Player::Player(JSON::SteamPlayer::Player &player) : inventory(player.steamid) {
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
    if (r.status_code != 200) {
        std::stringstream logss;
        logss << "Failed to fetch hours for " << steamid << " (" << r.status_code << ")";
        consoleLog(logss.str(), SEVERITY::INFO);
        return;
    }

    JSON::SteamGames::SteamGames userGames = JSON::SteamGames::fromJson(r.text);
    if (!userGames.response.games.has_value()) return;

    for (auto &game : userGames.response.games.value()) {
        if (game.appid == 440) {
            hours = game.playtime_forever / 60;
            return;
        }
    }
}

void Player::getLevel() {
    std::stringstream ss;
    ss << "https://api.steampowered.com/IPlayerService/GetSteamLevel/v1/?key=" << GLOBALS::scanner.config.apikey;
    ss << "&steamid=" << steamid;

    auto r = cpr::Get(cpr::Url{ss.str()});
    if (r.status_code != 200) {
        std::stringstream logss;
        logss << "Failed to fetch level for " << steamid << " (" << r.status_code << ")";
        consoleLog(logss.str(), SEVERITY::INFO);
        return;
    }
    
    JSON::SteamLevel::SteamLevel userLevel = JSON::SteamLevel::fromJson(r.text);

    if (!userLevel.response.player_level.has_value()) return;
    level = userLevel.response.player_level.value();
}

void Player::getHistories() {
    std::stringstream ss;
    ss << "https://backpack.tf/profiles/" << steamid;

    auto r = cpr::Get(cpr::Url{ss.str()});
    if (r.status_code != 200) {
        std::stringstream logss;
        logss << "Failed to fetch histories for " << steamid << " (" << r.status_code << ")";
        consoleLog(logss.str(), SEVERITY::INFO);
        return;
    }
    // TODO: find postion of "quantitySeries" and the first line break that follow instead of this
    // should hopefully be much faster
    std::string pageHtml = r.text;
    boost::regex quantityRegex("quantitySeries:.*\\]\n");
    boost::smatch quantityMatch;
    boost::regex_search(pageHtml, quantityMatch, quantityRegex);
    std::string quantityString = quantityMatch.str();

    std::string::difference_type quantityCount = std::count(quantityString.begin(), quantityString.end(), ',');

    badges = 0;
    std::string::size_type pos = 0;
    std::string target = "\"user-badge";
    while ((pos = pageHtml.find(target, pos)) != std::string::npos) {
        ++badges;
        pos += target.length();
    }

    histories = (quantityCount - 1) / 2;
}

// void getLevel();
// void getHistories();
// void getHours();

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
