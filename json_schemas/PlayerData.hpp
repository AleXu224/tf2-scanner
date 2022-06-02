//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     PlayerData data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann
{
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>>
    {
        static void to_json(json &j, const std::shared_ptr<T> &opt)
        {
            if (!opt)
                j = nullptr;
            else
                j = *opt;
        }

        static std::shared_ptr<T> from_json(const json &j)
        {
            if (j.is_null())
                return std::unique_ptr<T>();
            else
                return std::unique_ptr<T>(new T(j.get<T>()));
        }
    };
}
#endif

namespace JsonPlayer
{
    using nlohmann::json;

    inline json get_untyped(const json &j, const char *property)
    {
        if (j.find(property) != j.end())
        {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json &j, std::string property)
    {
        return get_untyped(j, property.data());
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json &j, const char *property)
    {
        if (j.find(property) != j.end())
        {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json &j, std::string property)
    {
        return get_optional<T>(j, property.data());
    }

    struct Player
    {
        std::string steamid;
        int64_t communityvisibilitystate;
        int64_t profilestate;
        std::string personaname;
        // std::shared_ptr<int64_t> commentpermission;
        std::string profileurl;
        // std::string avatar;
        std::string avatarmedium;
        // std::string avatarfull;
        // std::string avatarhash;
        // int64_t personastate;
        // std::string primaryclanid;
        // int64_t timecreated;
        // int64_t personastateflags;
        // std::shared_ptr<std::string> gameextrainfo;
        // std::shared_ptr<std::string> gameid;
        // std::shared_ptr<std::string> realname;
        // std::shared_ptr<std::string> loccountrycode;
        // std::shared_ptr<std::string> locstatecode;
        // std::shared_ptr<int64_t> loccityid;
        // std::shared_ptr<std::string> gameserverip;
        // std::shared_ptr<std::string> gameserversteamid;
        // std::shared_ptr<std::string> lobbysteamid;
    };

    struct Response
    {
        std::vector<Player> players;
    };

    struct PlayerData
    {
        Response response;
    };
}

namespace nlohmann
{
    void from_json(const json &j, JsonPlayer::Player &x);
    void to_json(json &j, const JsonPlayer::Player &x);

    void from_json(const json &j, JsonPlayer::Response &x);
    void to_json(json &j, const JsonPlayer::Response &x);

    void from_json(const json &j, JsonPlayer::PlayerData &x);
    void to_json(json &j, const JsonPlayer::PlayerData &x);

    inline void from_json(const json &j, JsonPlayer::Player &x)
    {
        x.steamid = j.at("steamid").get<std::string>();
        x.communityvisibilitystate = j.at("communityvisibilitystate").get<int64_t>();
        x.profilestate = j.at("profilestate").get<int64_t>();
        x.personaname = j.at("personaname").get<std::string>();
        // x.commentpermission = JsonPlayer::get_optional<int64_t>(j, "commentpermission");
        x.profileurl = j.at("profileurl").get<std::string>();
        // x.avatar = j.at("avatar").get<std::string>();
        x.avatarmedium = j.at("avatarmedium").get<std::string>();
        // x.avatarfull = j.at("avatarfull").get<std::string>();
        // x.avatarhash = j.at("avatarhash").get<std::string>();
        // x.personastate = j.at("personastate").get<int64_t>();
        // x.primaryclanid = j.at("primaryclanid").get<std::string>();
        // x.timecreated = j.at("timecreated").get<int64_t>();
        // x.personastateflags = j.at("personastateflags").get<int64_t>();
        // x.gameextrainfo = JsonPlayer::get_optional<std::string>(j, "gameextrainfo");
        // x.gameid = JsonPlayer::get_optional<std::string>(j, "gameid");
        // x.realname = JsonPlayer::get_optional<std::string>(j, "realname");
        // x.loccountrycode = JsonPlayer::get_optional<std::string>(j, "loccountrycode");
        // x.locstatecode = JsonPlayer::get_optional<std::string>(j, "locstatecode");
        // x.loccityid = JsonPlayer::get_optional<int64_t>(j, "loccityid");
        // x.gameserverip = JsonPlayer::get_optional<std::string>(j, "gameserverip");
        // x.gameserversteamid = JsonPlayer::get_optional<std::string>(j, "gameserversteamid");
        // x.lobbysteamid = JsonPlayer::get_optional<std::string>(j, "lobbysteamid");
    }

    inline void to_json(json &j, const JsonPlayer::Player &x)
    {
        j = json::object();
        j["steamid"] = x.steamid;
        j["communityvisibilitystate"] = x.communityvisibilitystate;
        j["profilestate"] = x.profilestate;
        j["personaname"] = x.personaname;
        // j["commentpermission"] = x.commentpermission;
        j["profileurl"] = x.profileurl;
        // j["avatar"] = x.avatar;
        j["avatarmedium"] = x.avatarmedium;
        // j["avatarfull"] = x.avatarfull;
        // j["avatarhash"] = x.avatarhash;
        // j["personastate"] = x.personastate;
        // j["primaryclanid"] = x.primaryclanid;
        // j["timecreated"] = x.timecreated;
        // j["personastateflags"] = x.personastateflags;
        // j["gameextrainfo"] = x.gameextrainfo;
        // j["gameid"] = x.gameid;
        // j["realname"] = x.realname;
        // j["loccountrycode"] = x.loccountrycode;
        // j["locstatecode"] = x.locstatecode;
        // j["loccityid"] = x.loccityid;
        // j["gameserverip"] = x.gameserverip;
        // j["gameserversteamid"] = x.gameserversteamid;
        // j["lobbysteamid"] = x.lobbysteamid;
    }

    inline void from_json(const json &j, JsonPlayer::Response &x)
    {
        x.players = j.at("players").get<std::vector<JsonPlayer::Player>>();
    }

    inline void to_json(json &j, const JsonPlayer::Response &x)
    {
        j = json::object();
        j["players"] = x.players;
    }

    inline void from_json(const json &j, JsonPlayer::PlayerData &x)
    {
        x.response = j.at("response").get<JsonPlayer::Response>();
    }

    inline void to_json(json &j, const JsonPlayer::PlayerData &x)
    {
        j = json::object();
        j["response"] = x.response;
    }
}
