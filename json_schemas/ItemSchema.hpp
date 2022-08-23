#pragma once
#include "assert.h"
#include "optional"
#include "string"
#include "vector"
#include "yyjson.h"

namespace JSON::Schema {
class Item {
   public:
    int defindex;
    std::string item_name;
    bool proper_name;

    Item(yyjson_val *obj) {
        this->defindex = yyjson_get_int(yyjson_obj_get(obj, "defindex"));
        this->item_name = yyjson_get_str(yyjson_obj_get(obj, "item_name"));
        this->proper_name = yyjson_get_bool(yyjson_obj_get(obj, "proper_name"));
    }
};

class Result {
   public:
    std::vector<Item> items;
    std::optional<int> next;

    Result(yyjson_val *obj) {
        yyjson_val *iValue;
        yyjson_arr_iter iterator;

        yyjson_arr_iter_init(yyjson_obj_get(obj, "items"), &iterator);
        while ((iValue = yyjson_arr_iter_next(&iterator))) {
            this->items.push_back(Item(iValue));
        }

        auto next = yyjson_obj_get(obj, "next");
        this->next = next ? std::make_optional(yyjson_get_int(next)) : std::nullopt;
    }
};

class Response {
   public:
    Result result;

    Response(yyjson_val *obj) : result(yyjson_obj_get(obj, "result")) {}
};

static Response fromJson(std::string &json) {
    auto doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    auto root = yyjson_doc_get_root(doc);
    auto schema = Response(root);
    yyjson_doc_free(doc);

    return schema;
}

static std::string toJson(Response &schema) {
    yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    {
        yyjson_mut_val *result = yyjson_mut_obj(doc);
        yyjson_mut_obj_add_val(doc, root, "result", result);
        {
            // pain
            yyjson_mut_val *items = yyjson_mut_arr(doc);
            yyjson_mut_obj_add_val(doc, result, "items", items);
            {
                for (auto &item : schema.result.items) {
                    yyjson_mut_val *itemValue = yyjson_mut_obj(doc);
                    yyjson_mut_obj_add_int(doc, itemValue, "defindex", item.defindex);
                    yyjson_mut_obj_add_str(doc, itemValue, "item_name", item.item_name.data());
                    yyjson_mut_obj_add_bool(doc, itemValue, "proper_name", item.proper_name);
                    yyjson_mut_arr_add_val(items, itemValue);
                }
            }
        }
    }

    auto jsonStr = yyjson_mut_write(doc, 0, nullptr);
    yyjson_mut_doc_free(doc);
    return jsonStr;
}
}  // namespace JSON::Schema
