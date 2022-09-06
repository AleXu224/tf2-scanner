#pragma once
#include "assert.h"
#include "optional"
#include "string"
#include "yyjson.h"

namespace JSON::SteamLevel {
class Response {
   public:
    std::optional<int> player_level;

    Response(yyjson_val *obj) {
        auto levelObj = yyjson_obj_get(obj, "player_level");
        if (levelObj && !yyjson_is_null(levelObj)) {
            this->player_level = yyjson_get_int(levelObj);
        }
    }
};

class SteamLevel {
    public:
    Response response;

    SteamLevel(yyjson_val *obj) : response(yyjson_obj_get(obj, "response")) {}
};

static SteamLevel fromJson(std::string &json) {
    yyjson_doc *doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    yyjson_val *root = yyjson_doc_get_root(doc);
    SteamLevel steamLevel(root);
    yyjson_doc_free(doc);

    return steamLevel;
}
}  // namespace JSON::SteamLevel