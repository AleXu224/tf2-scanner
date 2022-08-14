#ifndef RAYLIBTESTING_SIDEBAR_HPP
#define RAYLIBTESTING_SIDEBAR_HPP

#include "string"
#include "Overlay.hpp"
#include "../utilities/Animation.hpp"
#include "optional"

class SideBar : public OverlayBase {
    private:
    Transition transition;
    public:
    SideBar() : transition(0.0f, 1.0f, 0.2f, TransitionType::easeInOutCubic) {}
    void draw();
};

enum class InputType {
    INT,
    FLOAT,
    TEXT,
    TEXTMULTILINE
};

class CustomInput {
   private:
    InputType inputType;
    std::string inputName;
    std::string toolTip;
    bool isActive = false;
    Transition transition{0.0f, 1.0f, 0.2f, TransitionType::easeInOutCubic};

    int maxSize = 0;
    std::optional<float *> inputValueFloat;
    std::optional<int *> inputValueInt;
    std::optional<char *> inputValueText;
    std::optional<std::string *> inputValueTextMultiline;
    float width = 0.0f;
    float height = 0.0f;

   public:
    CustomInput(std::string inputName, int &inputValue, std::string toolTip = "") {
        this->inputType = InputType::INT;
        this->inputName = inputName;
        this->toolTip = toolTip;
        this->inputValueInt = &inputValue;
    }
    CustomInput(std::string inputName, char *inputValue, int maxSize, std::string toolTip = "") {
        this->inputType = InputType::TEXT;
        this->inputName = inputName;
        this->toolTip = toolTip;
        this->inputValueText = inputValue;
        this->maxSize = maxSize;
    }
    CustomInput(std::string inputName, float &inputValue, std::string toolTip = "") {
        this->inputType = InputType::FLOAT;
        this->inputName = inputName;
        this->toolTip = toolTip;
        this->inputValueFloat = &inputValue;
    }
    CustomInput(std::string inputName, std::string &inputValue, float width, float height) {
        this->inputType = InputType::TEXTMULTILINE;
        this->inputName = inputName;
        this->inputValueTextMultiline = &inputValue;
        this->width = width;
        this->height = height;
    }

    void draw();
};

bool CustomTextButton(const std::string& value);

#endif  // RAYLIBTESTING_SIDEBAR_HPP
