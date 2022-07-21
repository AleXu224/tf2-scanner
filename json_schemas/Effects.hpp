//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Effects data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

namespace JsonEffects
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

    using Effects = std::map<std::string, int64_t>;
}

namespace JsonEffects
{
    using Effects = std::map<std::string, int64_t>;
}

namespace nlohmann
{
}
