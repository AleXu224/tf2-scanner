#pragma once
#include "assert.h"
#include "optional"
#include "string"
#include "vector"
#include "yyjson.h"

namespace JSON::SteamPlayer {
class Player {
   public:
    std::string steamid;
    int communityvisibilitystate;
    std::optional<int> profilestate;
    std::string personaname;
    std::string profileurl;
    std::string avatarmedium;

    Player(yyjson_val *obj) {
        this->steamid = yyjson_get_str(yyjson_obj_get(obj, "steamid"));
        this->communityvisibilitystate = yyjson_get_int(yyjson_obj_get(obj, "communityvisibilitystate"));
        this->profilestate = yyjson_obj_get(obj, "profilestate") ? std::make_optional(yyjson_get_int(yyjson_obj_get(obj, "profilestate"))) : std::nullopt;
        this->personaname = yyjson_get_str(yyjson_obj_get(obj, "personaname"));
        this->profileurl = yyjson_get_str(yyjson_obj_get(obj, "profileurl"));
        this->avatarmedium = yyjson_get_str(yyjson_obj_get(obj, "avatarmedium"));
    }
};

class Response {
   public:
    std::vector<Player> players;

    Response(yyjson_val *obj) {
        yyjson_val *iValue;
        yyjson_arr_iter iterator;

        yyjson_arr_iter_init(yyjson_obj_get(obj, "players"), &iterator);
        while ((iValue = yyjson_arr_iter_next(&iterator))) {
            this->players.push_back(Player(iValue));
        }
    }
};

class SteamPlayer {
   public:
    Response response;

    SteamPlayer(yyjson_val *obj) : response(yyjson_obj_get(obj, "response")) {}
};

static SteamPlayer fromJson(std::string &json) {
    auto doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    auto root = yyjson_doc_get_root(doc);
    auto schema = SteamPlayer(root);
    yyjson_doc_free(doc);

    return schema;
}
}  // namespace JSON::SteamPlayer