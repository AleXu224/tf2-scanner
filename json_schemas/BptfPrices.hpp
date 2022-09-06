#pragma once
#include "assert.h"
#include "map"
#include "unordered_map"
#include "optional"
#include "string"
#include "vector"
#include "yyjson.h"
#include "variant"

namespace JSON::BptfPrices {
class Element {
   public:
    std::optional<double> value;
    std::optional<std::string> currency;

    Element(yyjson_val *obj) {
        auto currencyObj = yyjson_obj_get(obj, "currency");
        if (currencyObj && !yyjson_is_null(currencyObj)) {
            this->currency = std::make_optional(yyjson_get_str(currencyObj));
        } else {
            this->currency = std::nullopt;
        }

        auto valueObj = yyjson_obj_get(obj, "value");
        if (valueObj && !yyjson_is_null(valueObj)) {
            this->value = std::make_optional(yyjson_get_real(valueObj));
        } else {
            this->value = std::nullopt;
        }
    }
};

class Tradable {
   public:
    std::optional<std::variant<std::map<std::string, Element>, std::vector<Element>>> craftable;
    std::optional<std::variant<std::map<std::string, Element>, std::vector<Element>>> non_craftable;

    Tradable(yyjson_val *obj) {
        auto craftableObj = yyjson_obj_get(obj, "Craftable");
        if (!yyjson_is_null(craftableObj)) {
            if (yyjson_is_arr(craftableObj)) {
                craftable = std::vector<Element>{};
                yyjson_val *val;
                yyjson_arr_iter iter;

                yyjson_arr_iter_init(craftableObj, &iter);
                while ((val = yyjson_arr_iter_next(&iter))) {
                    std::get<std::vector<Element>>(craftable.value()).push_back(Element(val));
                }

            } else {
                // Map
                craftable = std::map<std::string, Element>{};

                yyjson_val *key, *val;
                yyjson_obj_iter iter;
                yyjson_obj_iter_init(craftableObj, &iter);
                while ((key = yyjson_obj_iter_next(&iter))) {
                    std::get<std::map<std::string, Element>>(craftable.value()).insert({yyjson_get_str(key), Element(yyjson_obj_iter_get_val(key))});
                }
            }
        }
        auto non_craftableObj = yyjson_obj_get(obj, "Non-Craftable");
        if (!yyjson_is_null(non_craftableObj)) {
            if (yyjson_is_arr(non_craftableObj)) {
                non_craftable = std::vector<Element>{};
                yyjson_val *val;
                yyjson_arr_iter iter;

                yyjson_arr_iter_init(non_craftableObj, &iter);
                while ((val = yyjson_arr_iter_next(&iter))) {
                    std::get<std::vector<Element>>(non_craftable.value()).push_back(Element(val));
                }

            } else {
                // Map
                non_craftable = std::map<std::string, Element>{};

                yyjson_val *key, *val;
                yyjson_obj_iter iter;
                yyjson_obj_iter_init(non_craftableObj, &iter);
                while ((key = yyjson_obj_iter_next(&iter))) {
                    std::get<std::map<std::string, Element>>(non_craftable.value()).insert({yyjson_get_str(key), Element(yyjson_obj_iter_get_val(key))});
                }
            }
        }

        // this->craftable = yyjson_obj_get(obj, "Craftable") ? std::make_optional(Element(yyjson_obj_get(obj, "Craftable"))) : std::nullopt;
        // this->non_craftable = yyjson_obj_get(obj, "Non-Craftable") ? std::make_optional(Element(yyjson_obj_get(obj, "Non-Craftable"))) : std::nullopt;
    }
};

class Price {
   public:
    Tradable tradable;

    Price(yyjson_val *obj) : tradable(yyjson_obj_get(obj, "Tradable")) {}
};

class Item {
   public:
    std::vector<int> defindex;
    std::unordered_map<std::string, Price> prices;

    Item(yyjson_val *obj) {
        {  // defindex
            yyjson_val *iValue;
            yyjson_arr_iter iterator;

            yyjson_arr_iter_init(yyjson_obj_get(obj, "defindex"), &iterator);
            while ((iValue = yyjson_arr_iter_next(&iterator))) {
                this->defindex.push_back(yyjson_get_int(iValue));
            }
        }

        {  // prices
            yyjson_val *key, *value;
            yyjson_obj_iter iterator;

            yyjson_obj_iter_init(yyjson_obj_get(obj, "prices"), &iterator);
            while ((key = yyjson_obj_iter_next(&iterator))) {
                value = yyjson_obj_iter_get_val(key);
                this->prices.insert({yyjson_get_str(key), Price(value)});
            }
        }
    }
};

class Response {
   public:
    int success;
    int current_time;
    double raw_usd_value;
    std::string usd_currency;
    int usd_currency_index;
    std::map<std::string, Item> items;

    Response(yyjson_val *obj) {
        this->success = yyjson_get_int(yyjson_obj_get(obj, "success"));
        this->current_time = yyjson_get_int(yyjson_obj_get(obj, "current_time"));
        this->raw_usd_value = yyjson_get_real(yyjson_obj_get(obj, "raw_usd_value"));
        this->usd_currency = yyjson_get_str(yyjson_obj_get(obj, "usd_currency"));
        this->usd_currency_index = yyjson_get_int(yyjson_obj_get(obj, "usd_currency_index"));
        {  // items
            yyjson_val *key, *value;
            yyjson_obj_iter iterator;

            yyjson_obj_iter_init(yyjson_obj_get(obj, "items"), &iterator);
            while ((key = yyjson_obj_iter_next(&iterator))) {
                value = yyjson_obj_iter_get_val(key);

                this->items.insert({yyjson_get_str(key), Item(value)});
            }
        }
    }
};

class BptfPrices {
   public:
    Response response;

    BptfPrices(yyjson_val *obj) : response(yyjson_obj_get(obj, "response")) {}
};

static BptfPrices fromJson(std::string &json) {
    auto doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    auto root = yyjson_doc_get_root(doc);
    auto bptfprices = BptfPrices(root);
    yyjson_doc_free(doc);

    return bptfprices;
}

static std::string toJson(BptfPrices &bptfprices) {
    yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    {  // root
        yyjson_mut_val *response = yyjson_mut_obj(doc);
        yyjson_mut_obj_add_val(doc, root, "response", response);
        {  // response
            yyjson_mut_val *success = yyjson_mut_int(doc, bptfprices.response.success);
            yyjson_mut_obj_add_val(doc, response, "success", success);

            yyjson_mut_val *current_time = yyjson_mut_int(doc, bptfprices.response.current_time);
            yyjson_mut_obj_add_val(doc, response, "current_time", current_time);

            yyjson_mut_val *raw_usd_value = yyjson_mut_real(doc, bptfprices.response.raw_usd_value);
            yyjson_mut_obj_add_val(doc, response, "raw_usd_value", raw_usd_value);

            yyjson_mut_val *usd_currency = yyjson_mut_str(doc, bptfprices.response.usd_currency.data());
            yyjson_mut_obj_add_val(doc, response, "usd_currency", usd_currency);

            yyjson_mut_val *usd_currency_index = yyjson_mut_int(doc, bptfprices.response.usd_currency_index);
            yyjson_mut_obj_add_val(doc, response, "usd_currency_index", usd_currency_index);

            yyjson_mut_val *items = yyjson_mut_obj(doc);
            yyjson_mut_obj_add_val(doc, response, "items", items);
            {  // items
                for (auto &itemsIterator : bptfprices.response.items) {
                    yyjson_mut_val *item = yyjson_mut_obj(doc);
                    {  // item
                        yyjson_mut_val *defindex = yyjson_mut_arr(doc);
                        {  // defindex
                            for (auto &defindexIterator : itemsIterator.second.defindex) {
                                yyjson_mut_val *defindexValue = yyjson_mut_int(doc, defindexIterator);
                                yyjson_mut_arr_add_val(defindex, defindexValue);
                            }
                        }  // defindex
                        yyjson_mut_obj_add_val(doc, item, "defindex", defindex);

                        yyjson_mut_val *prices = yyjson_mut_obj(doc);
                        yyjson_mut_obj_add_val(doc, item, "prices", prices);
                        {  // prices
                            for (auto &pricesIterator : itemsIterator.second.prices) {
                                yyjson_mut_val *price = yyjson_mut_obj(doc);
                                {
                                    yyjson_mut_val *tradable = yyjson_mut_obj(doc);
                                    {  // tradable
                                        if (pricesIterator.second.tradable.craftable.has_value()) {
                                            auto &elementParent = pricesIterator.second.tradable.craftable.value();
                                            if (std::holds_alternative<std::vector<Element>>(elementParent)) {
                                                auto &elementVector = std::get<std::vector<Element>>(elementParent);
                                                auto elementParentArr = yyjson_mut_arr(doc);

                                                for (auto &element : elementVector) {
                                                    yyjson_mut_val *elementValue = yyjson_mut_obj(doc);
                                                    {  // element
                                                        if (element.value.has_value()) {
                                                            yyjson_mut_val *value = yyjson_mut_real(doc, element.value.value());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "value", value);
                                                        }
                                                        if (element.currency.has_value()) {
                                                            yyjson_mut_val *currency = yyjson_mut_str(doc, element.currency.value().data());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "currency", currency);
                                                        }
                                                    }  // element
                                                    yyjson_mut_arr_add_val(elementParentArr, elementValue);
                                                }
                                                yyjson_mut_obj_add_val(doc, tradable, "Craftable", elementParentArr);
                                            }  // Craftable
                                            else if (std::holds_alternative<std::map<std::string, Element>>(elementParent)) {
                                                auto &elementMap = std::get<std::map<std::string, Element>>(elementParent);
                                                auto elementParentObj = yyjson_mut_obj(doc);

                                                for (auto &element : elementMap) {
                                                    yyjson_mut_val *elementValue = yyjson_mut_obj(doc);
                                                    {  // element
                                                        if (element.second.value.has_value()) {
                                                            yyjson_mut_val *value = yyjson_mut_real(doc, element.second.value.value());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "value", value);
                                                        }
                                                        if (element.second.currency.has_value()) {
                                                            yyjson_mut_val *currency = yyjson_mut_str(doc, element.second.currency.value().data());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "currency", currency);
                                                        }
                                                    }  // element
                                                    yyjson_mut_obj_add_val(doc, elementParentObj, element.first.data(), elementValue);
                                                }
                                                yyjson_mut_obj_add_val(doc, tradable, "Craftable", elementParentObj);
                                            }
                                        }
                                        if (pricesIterator.second.tradable.non_craftable.has_value()) {
                                            auto &elementParent = pricesIterator.second.tradable.non_craftable.value();
                                            if (std::holds_alternative<std::vector<Element>>(elementParent)) {
                                                auto &elementVector = std::get<std::vector<Element>>(elementParent);
                                                auto elementParentArr = yyjson_mut_arr(doc);

                                                for (auto &element : elementVector) {
                                                    yyjson_mut_val *elementValue = yyjson_mut_obj(doc);
                                                    {  // element
                                                        if (element.value.has_value()) {
                                                            yyjson_mut_val *value = yyjson_mut_real(doc, element.value.value());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "value", value);
                                                        }
                                                        if (element.currency.has_value()) {
                                                            yyjson_mut_val *currency = yyjson_mut_str(doc, element.currency.value().data());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "currency", currency);
                                                        }
                                                    }  // element
                                                    yyjson_mut_arr_add_val(elementParentArr, elementValue);
                                                }
                                                yyjson_mut_obj_add_val(doc, tradable, "Non-Craftable", elementParentArr);
                                            }  // NonCraftable
                                            else if (std::holds_alternative<std::map<std::string, Element>>(elementParent)) {
                                                auto &elementMap = std::get<std::map<std::string, Element>>(elementParent);
                                                auto elementParentObj = yyjson_mut_obj(doc);

                                                for (auto &element : elementMap) {
                                                    yyjson_mut_val *elementValue = yyjson_mut_obj(doc);
                                                    {  // element
                                                        if (element.second.value.has_value()) {
                                                            yyjson_mut_val *value = yyjson_mut_real(doc, element.second.value.value());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "value", value);
                                                        }
                                                        if (element.second.currency.has_value()) {
                                                            yyjson_mut_val *currency = yyjson_mut_str(doc, element.second.currency.value().data());
                                                            yyjson_mut_obj_add_val(doc, elementValue, "currency", currency);
                                                        }
                                                    }  // element
                                                    yyjson_mut_obj_add_val(doc, elementParentObj, element.first.data(), elementValue);
                                                }
                                                yyjson_mut_obj_add_val(doc, tradable, "Non-Craftable", elementParentObj);
                                            }
                                        }
                                    }  // tradable
                                    yyjson_mut_obj_add_val(doc, price, "Tradable", tradable);
                                }
                                yyjson_mut_obj_add_val(doc, prices, pricesIterator.first.data(), price);
                            }
                        }  // prices
                    }      // item
                    yyjson_mut_obj_add_val(doc, items, itemsIterator.first.data(), item);
                }
            }  // items
        }      // response
    }          // root

    std::string json = yyjson_mut_write(doc, 0, nullptr);
    yyjson_mut_doc_free(doc);
    return json;
}
}  // namespace JSON::BptfPrices