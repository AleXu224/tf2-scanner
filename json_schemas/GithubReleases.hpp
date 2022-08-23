#pragma once
#include "assert.h"
#include "string"
#include "yyjson.h"

namespace JSON::GithubReleases {
class GithubReleases {
   public:
    std::string html_url;
    std::string tag_name;

    GithubReleases(yyjson_val *obj) {
        html_url = yyjson_get_str(yyjson_obj_get(obj, "html_url"));
        tag_name = yyjson_get_str(yyjson_obj_get(obj, "tag_name"));
    }
};

static GithubReleases fromJson(std::string &json) {
    yyjson_doc *doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    yyjson_val *root = yyjson_doc_get_root(doc);
    GithubReleases githubReleases(root);
    yyjson_doc_free(doc);

    return githubReleases;
}
}  // namespace JSON::GithubReleases