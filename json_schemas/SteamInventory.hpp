#pragma once
#include "assert.h"
#include "optional"
#include "string"
#include "vector"
#include "yyjson.h"

namespace JSON::SteamInventory {
class Tag {
   public:
    std::string category;
    std::string internal_name;
    std::string localized_category_name;
    std::string localized_tag_name;
    std::optional<std::string> color;

    Tag(yyjson_val *obj) {
        this->category = yyjson_get_str(yyjson_obj_get(obj, "category"));
        this->internal_name = yyjson_get_str(yyjson_obj_get(obj, "internal_name"));
        this->localized_category_name = yyjson_get_str(yyjson_obj_get(obj, "localized_category_name"));
        this->localized_tag_name = yyjson_get_str(yyjson_obj_get(obj, "localized_tag_name"));
        this->color = yyjson_obj_get(obj, "color") ? std::make_optional(yyjson_get_str(yyjson_obj_get(obj, "color"))) : std::nullopt;
    }
};

class DescriptionDescription {
   public:
    std::string value;
    std::optional<std::string> color;

    DescriptionDescription(yyjson_val *obj) {
        this->value = yyjson_get_str(yyjson_obj_get(obj, "value"));
        this->color = yyjson_obj_get(obj, "color") ? std::make_optional(yyjson_get_str(yyjson_obj_get(obj, "color"))) : std::nullopt;
    }
};

class Description {
   public:
    std::string classid;
    std::string icon_url;
    std::optional<std::vector<DescriptionDescription>> descriptions;
    int tradable;
    std::string market_name;
    std::vector<Tag> tags;

    Description(yyjson_val *obj) {
        this->classid = yyjson_get_str(yyjson_obj_get(obj, "classid"));
        this->icon_url = yyjson_get_str(yyjson_obj_get(obj, "icon_url"));

        if (yyjson_obj_get(obj, "descriptions")) {
            this->descriptions = std::make_optional(std::vector<DescriptionDescription>());
            yyjson_val *iValue;
            yyjson_arr_iter iterator;

            yyjson_arr_iter_init(yyjson_obj_get(obj, "descriptions"), &iterator);
            while ((iValue = yyjson_arr_iter_next(&iterator))) {
                this->descriptions->push_back(DescriptionDescription(iValue));
            }
        }

        this->tradable = yyjson_get_int(yyjson_obj_get(obj, "tradable"));
        this->market_name = yyjson_get_str(yyjson_obj_get(obj, "market_name"));

        {  // tags
            yyjson_val *iValue;
            yyjson_arr_iter iterator;

            yyjson_arr_iter_init(yyjson_obj_get(obj, "tags"), &iterator);
            while ((iValue = yyjson_arr_iter_next(&iterator))) {
                this->tags.push_back(Tag(iValue));
            }
        }
    }
};

class Asset {
   public:
    std::string classid;

    Asset(yyjson_val *obj) {
        this->classid = yyjson_get_str(yyjson_obj_get(obj, "classid"));
    }
};

class SteamInventory {
   public:
    std::optional<std::vector<Asset>> assets;
    std::optional<std::vector<Description>> descriptions;
    int total_inventory_count;

    SteamInventory(yyjson_val *obj) {
        if (yyjson_obj_get(obj, "assets")) {
            this->assets = std::make_optional(std::vector<Asset>());
            yyjson_val *iValue;
            yyjson_arr_iter iterator;

            yyjson_arr_iter_init(yyjson_obj_get(obj, "assets"), &iterator);
            while ((iValue = yyjson_arr_iter_next(&iterator))) {
                this->assets->push_back(Asset(iValue));
            }
        }
        if (yyjson_obj_get(obj, "descriptions")) {
            this->descriptions = std::make_optional(std::vector<Description>());
            yyjson_val *iValue;
            yyjson_arr_iter iterator;

            yyjson_arr_iter_init(yyjson_obj_get(obj, "descriptions"), &iterator);
            while ((iValue = yyjson_arr_iter_next(&iterator))) {
                this->descriptions->push_back(Description(iValue));
            }
        }
        this->total_inventory_count = yyjson_get_int(yyjson_obj_get(obj, "total_inventory_count"));
    }
};

static SteamInventory fromJson(std::string &json) {
    auto doc = yyjson_read(json.data(), json.size(), 0);
    assert(doc);
    auto root = yyjson_doc_get_root(doc);
    auto inventory = SteamInventory(root);
    yyjson_doc_free(doc);

    return inventory;
}
}  // namespace JSON::SteamInventory