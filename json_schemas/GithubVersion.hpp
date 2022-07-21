//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     GithubVersion data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

namespace JsonGithubVersion {
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

struct GithubVersion {
    std::string html_url;
    std::string tag_name;
};
}  // namespace JsonGithubVersion

namespace nlohmann {
void from_json(const json& j, JsonGithubVersion::GithubVersion& x);
void to_json(json& j, const JsonGithubVersion::GithubVersion& x);

inline void from_json(const json& j, JsonGithubVersion::GithubVersion& x) {
    x.html_url = j.at("html_url").get<std::string>();
    x.tag_name = j.at("tag_name").get<std::string>();
}

inline void to_json(json& j, const JsonGithubVersion::GithubVersion& x) {
    j = json::object();
    j["html_url"] = x.html_url;
    j["tag_name"] = x.tag_name;
}
}  // namespace nlohmann
