#pragma once
#include "assert.h"
#include "map"
#include "string"
#include "yyjson.h"

namespace JSON::TfSkins {
class TfSkins {
   public:
    std::map<std::string, std::string> skins;

    TfSkins(yyjson_val *obj) {
        yyjson_val *key, *val;
        yyjson_obj_iter iter;

        yyjson_obj_iter_init(obj, &iter);
        while ((key = yyjson_obj_iter_next(&iter)) != NULL) {
            val = yyjson_obj_iter_get_val(key);
            skins.insert({yyjson_get_str(key), yyjson_get_str(val)});
        }
    }
};

static TfSkins fromJson(std::string &json) {
    auto doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    auto root = yyjson_doc_get_root(doc);
    auto skins = TfSkins(root);
    yyjson_doc_free(doc);

    return skins;
}

static std::string toJson(TfSkins &skins) {
    auto doc = yyjson_mut_doc_new(nullptr);
    auto root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    for (auto &skin : skins.skins) {
        yyjson_mut_obj_add_str(doc, root, skin.first.data(), skin.second.data());
    }

    std::string json = yyjson_mut_write(doc, 0, nullptr);
    yyjson_mut_doc_free(doc);
    return json;
}
}  // namespace JSON::TfSkins