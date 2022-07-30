#include "ApiKeyPrompt.hpp"
#include "imgui.h"
#include "../globals.hpp"
#include "SideBar.hpp"

using namespace ImGui;

void ApiKeyPrompt() {
    if (!GLOBALS::scanner.showApiKeyPrompt) return;

    ImGuiIO &io = ImGui::GetIO();

    const float fwidth = 360.0f;
    const float fheight = 139.0f;

    ImVec4 backgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
    PushStyleColor(ImGuiCol_ChildBg, COLORS::PRIMARY);

    SetNextWindowPos({0, 0}, ImGuiCond_Always);
    SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y}, ImGuiCond_Always);

    Begin("ApiKeyPrompt", nullptr, FLAGS - ImGuiWindowFlags_NoFocusOnAppearing);

    SetCursorPos({io.DisplaySize.x / 2 - fwidth / 2, io.DisplaySize.y / 2 - fheight / 2});
    BeginChild("ApiKeyPrompt", {fwidth, fheight});

    CustomTextInput("API Key", GLOBALS::scanner.config.apikey, 32);
    if (CustomTextButton("Save")) {
        auto &apiKey = GLOBALS::scanner.config.apikey;
        if (strlen(apiKey) == 32) {
            GLOBALS::scanner.showApiKeyPrompt = false;
            std::thread t([]() {
                GLOBALS::scanner.config.save();
                GLOBALS::scanner.config.fetchRequirements();
            });
            t.detach();
        }
    }

    EndChild();

    End();

    PopStyleVar(1);
    PopStyleColor(2);
}