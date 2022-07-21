//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Pricelist data = nlohmann::json::parse(jsonString);

#pragma once

#include <variant>
#include <nlohmann/json.hpp>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json & j, const std::shared_ptr<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json & j) {
            if (j.is_null()) return std::unique_ptr<T>(); else return std::unique_ptr<T>(new T(j.get<T>()));
        }
    };
}
#endif

namespace JsonPrices {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json & j, std::string property) {
        return get_optional<T>(j, property.data());
    }

    enum class Currency : int { HAT, KEYS, METAL, USD };

    struct CraftableElement {
        std::shared_ptr<double> value;
        std::shared_ptr<Currency> currency;
        // std::shared_ptr<double> difference;
        // std::shared_ptr<int64_t> last_update;
        // std::shared_ptr<double> value_high;
        // std::shared_ptr<bool> australium;
    };

    struct NonTradable {
        std::shared_ptr<std::vector<CraftableElement>> craftable;
        std::shared_ptr<std::vector<CraftableElement>> non_craftable;
    };

    using CraftableUnion = std::variant<std::vector<CraftableElement>, std::map<std::string, CraftableElement>>;

    struct Tradable {
        std::shared_ptr<CraftableUnion> craftable;
        std::shared_ptr<CraftableUnion> non_craftable;
    };

    struct Price {
        Tradable tradable;
        std::shared_ptr<NonTradable> non_tradable;
    };

    struct Item {
        std::vector<int64_t> defindex;
        std::map<std::string, Price> prices;
    };

    struct Response {
        int64_t success;
        int64_t current_time;
        double raw_usd_value;
        Currency usd_currency;
        int64_t usd_currency_index;
        std::map<std::string, Item> items;
    };

    struct Pricelist {
        Response response;
    };
}

namespace nlohmann {
    void from_json(const json & j, JsonPrices::CraftableElement & x);
    void to_json(json & j, const JsonPrices::CraftableElement & x);

    void from_json(const json & j, JsonPrices::NonTradable & x);
    void to_json(json & j, const JsonPrices::NonTradable & x);

    void from_json(const json & j, JsonPrices::Tradable & x);
    void to_json(json & j, const JsonPrices::Tradable & x);

    void from_json(const json & j, JsonPrices::Price & x);
    void to_json(json & j, const JsonPrices::Price & x);

    void from_json(const json & j, JsonPrices::Item & x);
    void to_json(json & j, const JsonPrices::Item & x);

    void from_json(const json & j, JsonPrices::Response & x);
    void to_json(json & j, const JsonPrices::Response & x);

    void from_json(const json & j, JsonPrices::Pricelist & x);
    void to_json(json & j, const JsonPrices::Pricelist & x);

    void from_json(const json & j, JsonPrices::Currency & x);
    void to_json(json & j, const JsonPrices::Currency & x);
    void from_json(const json & j, std::variant<std::vector<JsonPrices::CraftableElement>, std::map<std::string, JsonPrices::CraftableElement>> & x);
    void to_json(json & j, const std::variant<std::vector<JsonPrices::CraftableElement>, std::map<std::string, JsonPrices::CraftableElement>> & x);

    inline void from_json(const json & j, JsonPrices::CraftableElement& x) {
        x.value = JsonPrices::get_optional<double>(j, "value");
        x.currency = JsonPrices::get_optional<JsonPrices::Currency>(j, "currency");
        // x.difference = JsonPrices::get_optional<double>(j, "difference");
        // x.last_update = JsonPrices::get_optional<int64_t>(j, "last_update");
        // x.value_high = JsonPrices::get_optional<double>(j, "value_high");
        // x.australium = JsonPrices::get_optional<bool>(j, "australium");
    }

    inline void to_json(json & j, const JsonPrices::CraftableElement & x) {
        j = json::object();
        j["value"] = x.value;
        j["currency"] = x.currency;
        // j["difference"] = x.difference;
        // j["last_update"] = x.last_update;
        // j["value_high"] = x.value_high;
        // j["australium"] = x.australium;
    }

    inline void from_json(const json & j, JsonPrices::NonTradable& x) {
        x.craftable = JsonPrices::get_optional<std::vector<JsonPrices::CraftableElement>>(j, "Craftable");
        x.non_craftable = JsonPrices::get_optional<std::vector<JsonPrices::CraftableElement>>(j, "Non-Craftable");
    }

    inline void to_json(json & j, const JsonPrices::NonTradable & x) {
        j = json::object();
        j["Craftable"] = x.craftable;
        j["Non-Craftable"] = x.non_craftable;
    }

    inline void from_json(const json & j, JsonPrices::Tradable& x) {
        x.craftable = JsonPrices::get_optional<std::variant<std::vector<JsonPrices::CraftableElement>, std::map<std::string, JsonPrices::CraftableElement>>>(j, "Craftable");
        x.non_craftable = JsonPrices::get_optional<std::variant<std::vector<JsonPrices::CraftableElement>, std::map<std::string, JsonPrices::CraftableElement>>>(j, "Non-Craftable");
    }

    inline void to_json(json & j, const JsonPrices::Tradable & x) {
        j = json::object();
        j["Craftable"] = x.craftable;
        j["Non-Craftable"] = x.non_craftable;
    }

    inline void from_json(const json & j, JsonPrices::Price& x) {
        x.tradable = j.at("Tradable").get<JsonPrices::Tradable>();
        x.non_tradable = JsonPrices::get_optional<JsonPrices::NonTradable>(j, "Non-Tradable");
    }

    inline void to_json(json & j, const JsonPrices::Price & x) {
        j = json::object();
        j["Tradable"] = x.tradable;
        j["Non-Tradable"] = x.non_tradable;
    }

    inline void from_json(const json & j, JsonPrices::Item& x) {
        x.defindex = j.at("defindex").get<std::vector<int64_t>>();
        x.prices = j.at("prices").get<std::map<std::string, JsonPrices::Price>>();
    }

    inline void to_json(json & j, const JsonPrices::Item & x) {
        j = json::object();
        j["defindex"] = x.defindex;
        j["prices"] = x.prices;
    }

    inline void from_json(const json & j, JsonPrices::Response& x) {
        x.success = j.at("success").get<int64_t>();
        x.current_time = j.at("current_time").get<int64_t>();
        x.raw_usd_value = j.at("raw_usd_value").get<double>();
        x.usd_currency = j.at("usd_currency").get<JsonPrices::Currency>();
        x.usd_currency_index = j.at("usd_currency_index").get<int64_t>();
        x.items = j.at("items").get<std::map<std::string, JsonPrices::Item>>();
    }

    inline void to_json(json & j, const JsonPrices::Response & x) {
        j = json::object();
        j["success"] = x.success;
        j["current_time"] = x.current_time;
        j["raw_usd_value"] = x.raw_usd_value;
        j["usd_currency"] = x.usd_currency;
        j["usd_currency_index"] = x.usd_currency_index;
        j["items"] = x.items;
    }

    inline void from_json(const json & j, JsonPrices::Pricelist& x) {
        x.response = j.at("response").get<JsonPrices::Response>();
    }

    inline void to_json(json & j, const JsonPrices::Pricelist & x) {
        j = json::object();
        j["response"] = x.response;
    }

    inline void from_json(const json & j, JsonPrices::Currency & x) {
        if (j == "hat") x = JsonPrices::Currency::HAT;
        else if (j == "keys") x = JsonPrices::Currency::KEYS;
        else if (j == "metal") x = JsonPrices::Currency::METAL;
        else if (j == "usd") x = JsonPrices::Currency::USD;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const JsonPrices::Currency & x) {
        switch (x) {
            case JsonPrices::Currency::HAT: j = "hat"; break;
            case JsonPrices::Currency::KEYS: j = "keys"; break;
            case JsonPrices::Currency::METAL: j = "metal"; break;
            case JsonPrices::Currency::USD: j = "usd"; break;
            default: throw "This should not happen";
        }
    }
    inline void from_json(const json & j, std::variant<std::vector<JsonPrices::CraftableElement>, std::map<std::string, JsonPrices::CraftableElement>> & x) {
        if (j.is_object())
            x = j.get<std::map<std::string, JsonPrices::CraftableElement>>();
        else if (j.is_array())
            x = j.get<std::vector<JsonPrices::CraftableElement>>();
        else throw "Could not deserialize";
    }

    inline void to_json(json & j, const std::variant<std::vector<JsonPrices::CraftableElement>, std::map<std::string, JsonPrices::CraftableElement>> & x) {
        switch (x.index()) {
            case 0:
                j = std::get<std::vector<JsonPrices::CraftableElement>>(x);
                break;
            case 1:
                j = std::get<std::map<std::string, JsonPrices::CraftableElement>>(x);
                break;
            default: throw "Input JSON does not conform to schema";
        }
    }
}
