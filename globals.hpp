//
// Created by Squizell on 27/04/2022.
//

#ifndef RAYLIBTESTING_GLOBALS_HPP
#define RAYLIBTESTING_GLOBALS_HPP

#include "imgui.h"
#include "vector"
#include "scanning/Scanner.hpp"
#include "components/Console.hpp"
#include "string"

#define FLAGS 8455
//ImGuiWindowFlags window_flags = 0;
//window_flags |= ImGuiWindowFlags_NoTitleBar;
//window_flags |= ImGuiWindowFlags_NoResize;
//window_flags |= ImGuiWindowFlags_NoMove;
//window_flags |= ImGuiWindowFlags_NoSavedSettings;
//window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

class COLORS{
public:
    constexpr static ImVec4 PRIMARY = ImVec4(33.0f/255, 33.0f/255, 33.0f/255, 1.0f);
    constexpr static ImVec4 PRIMARY_LIGHT = ImVec4(44.0f/255, 44.0f/255, 44.0f/255, 1.0f);
    constexpr static ImVec4 BACKGROUND = ImVec4(13.0f/255, 13.0f/255, 13.0f/255, 1.0f);
    constexpr static ImVec4 TEXT = ImVec4(184.0f/255, 184.0f/255, 184.0f/255, 1.0f);
    constexpr static ImVec4 SECONDARY = ImVec4(35.0f/255, 198.0f/255, 129.0f/255, 1.0f);

};

class GLOBALS {
public:
    static std::vector<ImFont*> FONTS;
    static Console console;
    static Scanner scanner;
};

#define CURRENCY_SCRAP "Scrap Metal"
#define CURRENCY_RECLAIMED "Reclaimed Metal"
#define CURRENCY_REFINED "Refined Metal"
#define CURRENCY_KEY "Mann Co. Supply Crate Key"

float BezierBlend(float t);

#define ROBOTO_12 0
#define ROBOTO_10 1
#define ROBOTO_16 2
#define ICONS 3


#endif //RAYLIBTESTING_GLOBALS_HPP
