#include "home.hpp"
#include "core/app.hpp"
#include "widgets/container.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/text.hpp"
#include "widgets/topNav.hpp"

namespace ui {
	void Home::State::initState() {
		// Simulate loading for 2 seconds
		std::thread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			setState([this]() {
				isLoading = false;
			});
			this->element->getApp()->inputQueue.push(StateChange{});
		}).detach();
	}

	squi::core::Child Home::State::build(const Element &element) {
		if (isLoading) {
			return Container{
				.child = Text{
					.widget{
						.alignment = Alignment::Center,
					},
					.text = "Loading...",
					.fontSize = 24,
				},
			};
		}

		return TopNav{
			.pages{
				TopNav::Page{
					.name = "Home",
					.content = ScrollView{
						.children{},
					},
				},
			},
		};
	}
}// namespace ui