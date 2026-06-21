#pragma once

#include "core/core.hpp"

namespace ui {
	using namespace squi;
	struct Home : StatefulWidget {
		// Args
		Key key;
		Args widget{};

		struct State : WidgetState<Home> {
			bool isLoading = true;

			void initState() override;

			Child build(const Element &element) override;
		};
	};
}// namespace ui