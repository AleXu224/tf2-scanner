#pragma once
#include "yyjson.h"
#include "map"
#include "string"
#include "assert.h"

namespace JSON::TFEffects {
class TFEffects {
   public:
    std::map<std::string, int> effects;

    TFEffects(yyjson_val *obj) {
        yyjson_val *key, *val;
        yyjson_obj_iter iter;

        yyjson_obj_iter_init(obj, &iter);
        while ((key = yyjson_obj_iter_next(&iter)) != NULL) {
            val = yyjson_obj_iter_get_val(key);
            effects.insert({yyjson_get_str(key), yyjson_get_int(val)});
        }
    }
};

static TFEffects fromJson(std::string &json) {
    auto doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    auto root = yyjson_doc_get_root(doc);
    auto effects = TFEffects(root);
    yyjson_doc_free(doc);

    return effects;
}

static std::string toJson(TFEffects &effects) {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    for (auto &effect : effects.effects) {
        yyjson_mut_obj_add_int(doc, root, effect.first.data(), effect.second);
    }

    std::string json = yyjson_mut_write(doc, 0, nullptr);
    yyjson_mut_doc_free(doc);
    return json;
}
}  // namespace JSON::TFEffects