#pragma once

#include <optional>
#include <string>
#include <vector>

namespace Serialize::GetPlayerSummaries {
    struct Player {
        std::string steamid;
        int64_t communityvisibilitystate;
        int64_t profilestate;
        std::string personaname;
        std::string profileurl;
        std::string avatar;
        std::string avatarmedium;
        std::string avatarfull;
        std::string avatarhash;
        int64_t personastate;
        std::optional<std::string> realname;
        std::optional<std::string> primaryclanid;
        std::optional<int64_t> timecreated;
        std::optional<int64_t> personastateflags;
        std::optional<std::string> loccountrycode;
    };

    struct Response {
        std::vector<Player> players;
    };

    struct Data {
        Response response;
    };
}
