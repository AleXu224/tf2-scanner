//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     MarketPrices data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

namespace MarketPricesJson {
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

struct MarketPrice {
    std::string name;
    int64_t sell_price;
};

using MarketPrices = std::vector<MarketPrice>;
}  // namespace MarketPricesJson

namespace MarketPricesJson {
using MarketPrices = std::vector<MarketPrice>;
}

namespace nlohmann {
void from_json(const json& j, MarketPricesJson::MarketPrice& x);
void to_json(json& j, const MarketPricesJson::MarketPrice& x);

inline void from_json(const json& j, MarketPricesJson::MarketPrice& x) {
    x.name = j.at("name").get<std::string>();
    x.sell_price = j.at("sell_price").get<int64_t>();
}

inline void to_json(json& j, const MarketPricesJson::MarketPrice& x) {
    j = json::object();
    j["name"] = x.name;
    j["sell_price"] = x.sell_price;
}
}  // namespace nlohmann
