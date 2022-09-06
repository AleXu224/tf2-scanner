#pragma once
#include "assert.h"
#include "string"
#include "yyjson.h"
#include "optional"
#include "vector"

namespace JSON::SteamGames {
class Game {
   public:
    int appid;
    int playtime_forever;

    Game(yyjson_val *obj) {
        this->appid = yyjson_get_int(yyjson_obj_get(obj, "appid"));
        this->playtime_forever = yyjson_get_int(yyjson_obj_get(obj, "playtime_forever"));
    }
};

class Response {
    public:
    std::optional<std::vector<Game>> games;

    Response(yyjson_val *obj) {
        auto gamesObj = yyjson_obj_get(obj, "games");
        if (gamesObj && !yyjson_is_null(gamesObj)) {
            games = std::vector<Game>();

            yyjson_val *gameObj;
            yyjson_arr_iter iter;

            yyjson_arr_iter_init(gamesObj, &iter);
            while((gameObj = yyjson_arr_iter_next(&iter))) {
                games->emplace_back(Game(gameObj));
            }
        }
    }
};

class SteamGames {
    public:
    Response response;

    SteamGames(yyjson_val *obj) : response(yyjson_obj_get(obj, "response")) {}
};

static SteamGames fromJson(std::string &json) {
    yyjson_doc *doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    yyjson_val *root = yyjson_doc_get_root(doc);
    SteamGames steamGames(root);
    yyjson_doc_free(doc);

    return steamGames;
}
}  // namespace JSON::SteamGames