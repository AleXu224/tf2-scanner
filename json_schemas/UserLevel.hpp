//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     UserLevels data = nlohmann::json::parse(jsonString);

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

namespace JsonUserLevel {
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

struct Response {
    std::shared_ptr<int64_t> player_level;
};

struct UserLevel {
    Response response;
};

using UserLevels = std::vector<UserLevel>;
}  // namespace JsonUserLevel

namespace JsonUserLevel {
using UserLevels = std::vector<UserLevel>;
}

namespace nlohmann {
void from_json(const json& j, JsonUserLevel::Response& x);
void to_json(json& j, const JsonUserLevel::Response& x);

void from_json(const json& j, JsonUserLevel::UserLevel& x);
void to_json(json& j, const JsonUserLevel::UserLevel& x);

inline void from_json(const json& j, JsonUserLevel::Response& x) {
    x.player_level = JsonUserLevel::get_optional<int64_t>(j, "player_level");
}

inline void to_json(json& j, const JsonUserLevel::Response& x) {
    j = json::object();
    j["player_level"] = x.player_level;
}

inline void from_json(const json& j, JsonUserLevel::UserLevel& x) {
    x.response = j.at("response").get<JsonUserLevel::Response>();
}

inline void to_json(json& j, const JsonUserLevel::UserLevel& x) {
    j = json::object();
    j["response"] = x.response;
}
}  // namespace nlohmann
