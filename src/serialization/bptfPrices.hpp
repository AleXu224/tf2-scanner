#pragma once

#include <map>
#include <optional>
#include <variant>

// https://raw.githubusercontent.com/AleXu224/bptf_pricelist/refs/heads/master/schema_bptf.json

namespace Serialize {
	struct BptfPrices {
		struct CraftableElement {
			std::optional<double> value;
			std::optional<std::string> currency;
			std::optional<double> difference;
			std::optional<int64_t> last_update;
			std::optional<double> value_high;
			std::optional<bool> australium;
		};

		struct NonTradable {
			std::optional<std::vector<CraftableElement>> craftable;
			std::optional<std::vector<CraftableElement>> non_craftable;
		};

		using CraftableUnion = std::variant<std::vector<CraftableElement>, std::map<std::string, CraftableElement>>;

		struct Tradable {
			std::optional<CraftableUnion> craftable;
			std::optional<CraftableUnion> non_craftable;
		};

		struct Price {
			Tradable tradable;
			std::optional<NonTradable> non_tradable;
		};

		struct Item {
			std::vector<int64_t> defindex;
			std::map<std::string, Price> prices;
		};

		struct Response {
			int64_t success;
			int64_t current_time;
			double raw_usd_value;
			std::string usd_currency;
			int64_t usd_currency_index;
			std::map<std::string, Item> items;
		};

		Response response;
	};
}// namespace Serialize
