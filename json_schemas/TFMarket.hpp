#pragma once
#include "assert.h"
#include "string"
#include "vector"
#include "yyjson.h"

namespace JSON::TFMarket {
class Price {
   public:
    std::string name;
    int sell_price;

    Price(yyjson_val *obj) {
        this->name = yyjson_get_str(yyjson_obj_get(obj, "name"));
        this->sell_price = yyjson_get_int(yyjson_obj_get(obj, "sell_price"));
    }
};

class TFMarket {
   public:
    std::vector<Price> prices;

    TFMarket(yyjson_val *obj) {
        yyjson_val *val;
        yyjson_arr_iter iter;

        yyjson_arr_iter_init(obj, &iter);
        while ((val = yyjson_arr_iter_next(&iter))) {
            prices.emplace_back(val);
        }
    }
};

static TFMarket fromJson(std::string &json) {
    auto *doc = yyjson_read(json.c_str(), json.size(), 0);
    assert(doc);
    auto *root = yyjson_doc_get_root(doc);
    TFMarket market(root);
    yyjson_doc_free(doc);

    return market;
}

static std::string toJson(TFMarket &market) {
    auto *doc = yyjson_mut_doc_new(nullptr);
    auto *root = yyjson_mut_arr(doc);
    yyjson_mut_doc_set_root(doc, root);

    for (auto &price : market.prices) {
        auto *priceObj = yyjson_mut_obj(doc);
        {
            yyjson_mut_obj_add_str(doc, priceObj, "name", price.name.data());
            yyjson_mut_obj_add_int(doc, priceObj, "sell_price", price.sell_price);
        }
        yyjson_mut_arr_add_val(root, priceObj);
    }

    std::string json = yyjson_mut_write(doc, 0, nullptr);
    yyjson_mut_doc_free(doc);

    return json;
}
}  // namespace JSON::TFMarket