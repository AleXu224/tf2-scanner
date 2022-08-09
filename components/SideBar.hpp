#ifndef RAYLIBTESTING_SIDEBAR_HPP
#define RAYLIBTESTING_SIDEBAR_HPP

#include "string"
#include "Overlay.hpp"
#include "../utilities/Animation.hpp"

class SideBar : public OverlayBase {
    private:
    Transition transition;
    public:
    SideBar() : transition(0.0f, 1.0f, 0.2f, TransitionType::easeInOutCubic) {}
    void draw();
};
void CustomTextInput(std::string input_name, char* input_value, int maxSize, std::string toolTip = "");
bool CustomTextButton(const std::string& value);

#endif  // RAYLIBTESTING_SIDEBAR_HPP
