//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     UserGamesLists data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
template <typename T>
struct adl_serializer<std::shared_ptr<T>> {
    static void to_json(json& j, const std::shared_ptr<T>& opt) {
        if (!opt)
            j = nullptr;
        else
            j = *opt;
    }

    static std::shared_ptr<T> from_json(const json& j) {
        if (j.is_null())
            return std::unique_ptr<T>();
        else
            return std::unique_ptr<T>(new T(j.get<T>()));
    }
};
}  // namespace nlohmann
#endif

namespace JsonUserGames {
using nlohmann::json;

inline json get_untyped(const json& j, const char* property) {
    if (j.find(property) != j.end()) {
        return j.at(property).get<json>();
    }
    return json();
}

inline json get_untyped(const json& j, std::string property) {
    return get_untyped(j, property.data());
}

template <typename T>
inline std::shared_ptr<T> get_optional(const json& j, const char* property) {
    if (j.find(property) != j.end()) {
        return j.at(property).get<std::shared_ptr<T>>();
    }
    return std::shared_ptr<T>();
}

template <typename T>
inline std::shared_ptr<T> get_optional(const json& j, std::string property) {
    return get_optional<T>(j, property.data());
}

struct Game {
    int64_t appid;
    std::string name;
    int64_t playtime_forever;
    std::string img_icon_url;
    std::shared_ptr<bool> has_community_visible_stats;
    int64_t playtime_windows_forever;
    int64_t playtime_mac_forever;
    int64_t playtime_linux_forever;
};

struct Response {
    std::shared_ptr<int64_t> game_count;
    std::shared_ptr<std::vector<Game>> games;
};

struct UserGamesList {
    Response response;
};

using UserGamesLists = std::vector<UserGamesList>;
}  // namespace JsonUserGames

namespace JsonUserGames {
using UserGamesLists = std::vector<UserGamesList>;
}

namespace nlohmann {
void from_json(const json& j, JsonUserGames::Game& x);
void to_json(json& j, const JsonUserGames::Game& x);

void from_json(const json& j, JsonUserGames::Response& x);
void to_json(json& j, const JsonUserGames::Response& x);

void from_json(const json& j, JsonUserGames::UserGamesList& x);
void to_json(json& j, const JsonUserGames::UserGamesList& x);

inline void from_json(const json& j, JsonUserGames::Game& x) {
    x.appid = j.at("appid").get<int64_t>();
    x.name = j.at("name").get<std::string>();
    x.playtime_forever = j.at("playtime_forever").get<int64_t>();
    x.img_icon_url = j.at("img_icon_url").get<std::string>();
    x.has_community_visible_stats = JsonUserGames::get_optional<bool>(j, "has_community_visible_stats");
    x.playtime_windows_forever = j.at("playtime_windows_forever").get<int64_t>();
    x.playtime_mac_forever = j.at("playtime_mac_forever").get<int64_t>();
    x.playtime_linux_forever = j.at("playtime_linux_forever").get<int64_t>();
}

inline void to_json(json& j, const JsonUserGames::Game& x) {
    j = json::object();
    j["appid"] = x.appid;
    j["name"] = x.name;
    j["playtime_forever"] = x.playtime_forever;
    j["img_icon_url"] = x.img_icon_url;
    j["has_community_visible_stats"] = x.has_community_visible_stats;
    j["playtime_windows_forever"] = x.playtime_windows_forever;
    j["playtime_mac_forever"] = x.playtime_mac_forever;
    j["playtime_linux_forever"] = x.playtime_linux_forever;
}

inline void from_json(const json& j, JsonUserGames::Response& x) {
    x.game_count = JsonUserGames::get_optional<int64_t>(j, "game_count");
    x.games = JsonUserGames::get_optional<std::vector<JsonUserGames::Game>>(j, "games");
}

inline void to_json(json& j, const JsonUserGames::Response& x) {
    j = json::object();
    j["game_count"] = x.game_count;
    j["games"] = x.games;
}

inline void from_json(const json& j, JsonUserGames::UserGamesList& x) {
    x.response = j.at("response").get<JsonUserGames::Response>();
}

inline void to_json(json& j, const JsonUserGames::UserGamesList& x) {
    j = json::object();
    j["response"] = x.response;
}
}  // namespace nlohmann
