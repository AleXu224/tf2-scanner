#include "core/app.hpp"
#include "core/core.hpp"

#include "ui/home.hpp"

int main() {
	using namespace squi;

	// auto resp = squi::Networking::get(
	// 	"https://steamcommunity.com/inventory/76561198143252042/440/2?l=english&count=2000"
	// );

	// Serialize::Inventory data;


	// auto start = std::chrono::steady_clock::now();
	// auto resp = squi::Networking::get(
	// 	"https://raw.githubusercontent.com/AleXu224/bptf_pricelist/refs/heads/master/schema_bptf.json"
	// );
	// auto networking = std::chrono::steady_clock::now();

	// Serialize::BptfPrices data;
	// auto d = glz::read<glz::opts{.error_on_unknown_keys = false}>(data, resp.body);
	// if (d) {
	// 	std::println("failed loading the file");
	// 	return 0;
	// }
	// auto deserialization = std::chrono::steady_clock::now();

	// std::println("networking: {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(networking - start).count());
	// std::println("deserialization: {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(deserialization - networking).count());

	// for (const auto &[key, item]: data.response.items) {
	// 	if (item.defindex.size() > 1) {
	// 		std::println("item: {}", key);

	// 		for (const auto &defindex: item.defindex) {
	// 			std::println("defindex: {}", defindex);
	// 		}
	// 	}
	// }


	App app{
		.windowOptions{
			.name = "Tf2 scanner",
			.width = 1280,
			.height = 720,
		},
		.child = ui::Home{},
	};
	app.initialize();
	App::runAllWindows();

	return 0;
}
