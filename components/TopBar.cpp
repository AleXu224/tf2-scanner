#include "TopBar.hpp"

#include "../globals.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "iostream"
#include "map"
#include "string"

using namespace ImGui;

struct ButtonStatus {
    double progress = 0.0f;  // 0.0 - 1.0
};

bool TopBarButton(const std::string &value) {
    // Invisible button and size calculation
    ImVec2 size = CalcTextSize(value.c_str(), nullptr, true);
    bool returnValue = InvisibleButton(value.c_str(), ImVec2(size.x + 16 * 2, 48));

    // Color calculation and transition
    ImGuiWindow *window = GetCurrentWindow();
    const ImGuiID id = window->GetID(value.c_str());
    static std::map<ImGuiID, ButtonStatus> buttons;
    ImGuiIO &io = ImGui::GetIO();
    ImVec4 buttonColor;
    ImVec4 textColor;
    if (IsItemHovered()) {
        buttons[id].progress += io.DeltaTime * (1.0f / 0.2f);
        if (buttons[id].progress > 1.0f) {
            buttons[id].progress = 1.0f;
        }
    } else {
        buttons[id].progress -= io.DeltaTime * (1.0f / 0.2f);
        if (buttons[id].progress < 0.0f) {
            buttons[id].progress = 0.0f;
        }
    }
    buttonColor = ImLerp(COLORS::PRIMARY_LIGHT, COLORS::SECONDARY, buttons[id].progress);
    textColor = ImLerp(COLORS::TEXT, COLORS::PRIMARY, buttons[id].progress);

    // Visible button and text
    ImDrawList *draw_list = GetWindowDrawList();
    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), ImColor(buttonColor));

    ImVec2 rectsize = GetItemRectSize();
    ImVec2 textPos = {
        GetItemRectMin().x + rectsize.x / 2 - size.x / 2,
        GetItemRectMin().y + rectsize.y / 2 - size.y / 2};
    draw_list->AddText(textPos, ImColor(textColor), value.c_str());

    return returnValue;
}

void TopBar() {
    // Styling
    PushStyleColor(ImGuiCol_WindowBg, COLORS::PRIMARY);
    PushStyleColor(ImGuiCol_Text, COLORS::TEXT);
    PushStyleColor(ImGuiCol_Button, COLORS::PRIMARY_LIGHT);
    PushStyleColor(ImGuiCol_ButtonHovered, COLORS::SECONDARY);

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16, 0));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    PushFont(GLOBALS::FONTS[ROBOTO_16]);

    // Window setup
    SetNextWindowPos(ImVec2(0, 0));
    SetNextWindowSize(ImVec2(GetIO().DisplaySize.x, 48));

    // Window
    Begin("Top Bar", nullptr, FLAGS);
    if (!GLOBALS::scanner.isScanning) {
        if (TopBarButton("Scan")) {
            std::thread scanThread([]() {
                GLOBALS::scanner.Scan();
            });
            scanThread.detach();
        }
    } else if (GLOBALS::scanner.isScanning && !GLOBALS::scanner.stopScanning) {
        if (TopBarButton("Stop Scan")) {
            GLOBALS::scanner.stopScanning = true;
        }
    } else if (GLOBALS::scanner.isScanning && GLOBALS::scanner.stopScanning) {
       TopBarButton("Stopping...");
    }

    if (GLOBALS::scanner.updateAvailable) {
        std::stringstream ss;
        ss << "Update available: " << GLOBALS::scanner.updateVersion;
        SameLine();
        SetCursorPosX(GetCursorPosX() + 8);
        if (TopBarButton(ss.str())) {
            ShellExecute(NULL, "open", GLOBALS::scanner.updateLink.c_str(), NULL, NULL, SW_SHOW);
        }
    }

    auto settingsOffset = 16 * 2 + CalcTextSize("Settings").x;
    SameLine(GetIO().DisplaySize.x - settingsOffset);
    if (TopBarButton("Settings")) {
        GLOBALS::scanner.showDrawer = true;
    }

    if (GLOBALS::scanner.playerList.size() > 0) {
        auto clearAllOffset = 16 * 2 + CalcTextSize("Clear All").x + 8;
        SameLine(GetIO().DisplaySize.x - clearAllOffset - settingsOffset);
        if (TopBarButton("Clear All")) {
            GLOBALS::scanner.playerList.clear();
        }
    }
    End();

    // Cleanup
    PopStyleColor(4);
    PopStyleVar(3);
    PopFont();
}
