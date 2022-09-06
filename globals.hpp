#ifndef RAYLIBTESTING_GLOBALS_HPP
#define RAYLIBTESTING_GLOBALS_HPP

#include "components/Console.hpp"
#include "imgui.h"
#include "scanning/Scanner.hpp"
#include "string"
#include "vector"

#define SCANNER_VERSION "2.4.0"

#define FLAGS 12551
// ImGuiWindowFlags window_flags = 0;
// window_flags |= ImGuiWindowFlags_NoTitleBar;
// window_flags |= ImGuiWindowFlags_NoResize;
// window_flags |= ImGuiWindowFlags_NoMove;
// window_flags |= ImGuiWindowFlags_NoSavedSettings;
// window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
// window_flags |= ImGuiWindowFlags_NoFocusOnAppearing;

class COLORS {
   public:
    constexpr static ImVec4 PRIMARY = ImVec4(33.0f / 255, 33.0f / 255, 33.0f / 255, 1.0f);
    constexpr static ImVec4 PRIMARY_LIGHT = ImVec4(44.0f / 255, 44.0f / 255, 44.0f / 255, 1.0f);
    constexpr static ImVec4 BACKGROUND = ImVec4(13.0f / 255, 13.0f / 255, 13.0f / 255, 1.0f);
    constexpr static ImVec4 TEXT = ImVec4(184.0f / 255, 184.0f / 255, 184.0f / 255, 1.0f);
    constexpr static ImVec4 SECONDARY = ImVec4(35.0f / 255, 198.0f / 255, 129.0f / 255, 1.0f);
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

#define ROBOTO_12 0
#define ROBOTO_10 1
#define ROBOTO_16 2
#define ROBOTO_14 3
#define ICONS 4

#endif  // RAYLIBTESTING_GLOBALS_HPP
