#include "networking.hpp"
#include "window.hpp"

#include "glaze/glaze.hpp"

#include "serialization/GetPlayerSummaries.hpp"

int main() {
	using namespace squi;

	auto resp = squi::Networking::get(
		"https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=CC612F27511EEF5B4C85654CB6B48099&format=json&steamids=76561199124649433,76561198827408448"
	);

	Serialize::GetPlayerSummaries::Data data;
	auto d = glz::read<glz::opts{.error_on_unknown_keys = false}>(data, resp.body);
	if (d) {
		std::println("failed loading the file");
		return 0;
	}

	auto window = Window{
		WindowOptions{
			.name = "Tf2 scanner",
			.width = 1280,
			.height = 720,
		}
	};

	window.run();

	return 0;
}
