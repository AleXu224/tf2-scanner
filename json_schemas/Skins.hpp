//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Skins data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

namespace JsonSkins
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

    using Skins = std::map<std::string, std::string>;
}

namespace JsonSkins
{
    using Skins = std::map<std::string, std::string>;
}

namespace nlohmann
{
}
