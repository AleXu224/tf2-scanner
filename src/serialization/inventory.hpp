#pragma once

#include <optional>
#include <string>
#include <vector>

// https://steamcommunity.com/inventory/76561198143252042/440/2?l=english&count=2000

namespace Serialize {
	struct Inventory {
		struct Asset {
			int64_t appid;
			std::string contextid;
			std::string assetid;
			std::string classid;
			std::string instanceid;
			std::string amount;
		};

		struct Action {
			std::string link;
			std::string name;
		};

		struct DescriptionDescription {
			std::string value;
			std::string name;
			std::optional<std::string> color;
			std::optional<std::string> type;
		};

		struct Tag {
			std::string category;
			std::string internal_name;
			std::string localized_category_name;
			std::string localized_tag_name;
			std::optional<std::string> color;
		};

		struct Description {
			int64_t appid;
			std::string classid;
			std::string instanceid;
			int64_t currency;
			std::string background_color;
			std::string icon_url;
			std::string icon_url_large;
			int64_t tradable;
			std::vector<Action> actions;
			std::string name;
			std::string name_color;
			std::string type;
			std::string market_name;
			std::string market_hash_name;
			int64_t commodity;
			int64_t market_tradable_restriction;
			int64_t market_marketable_restriction;
			int64_t marketable;
			std::vector<Tag> tags;
			std::string contextid;
			std::string assetid;
			std::optional<std::vector<DescriptionDescription>> descriptions;
			std::optional<std::vector<Action>> market_actions;
			std::optional<std::vector<std::string>> fraudwarnings;
		};

		std::optional<std::vector<Asset>> assets;
		std::optional<std::vector<Description>> descriptions;
		int64_t total_inventory_count;
		int64_t success;
		int64_t rwgrsn;
	};
}// namespace Serialize
