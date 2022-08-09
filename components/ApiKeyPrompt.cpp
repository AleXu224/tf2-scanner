#include "ApiKeyPrompt.hpp"
#include "imgui.h"
#include "../globals.hpp"
#include "SideBar.hpp"

using namespace ImGui;

void ApiKeyPrompt::draw() {
    if (!shouldClose) {
        transition.forward();
    } else {
        transition.backward();
    }

    if (transition.getProgress() == 0.0f) canClose = true;
    OverlayBackground(transition);
    ImGuiIO &io = ImGui::GetIO();

    const float fwidth = 360.0f;
    const float fheight = 139.0f;

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
    PushStyleColor(ImGuiCol_WindowBg, COLORS::PRIMARY);
    ImVec2 windowPos(io.DisplaySize.x / 2 - fwidth / 2, io.DisplaySize.y / 2 - fheight / 2);
    windowPos.y += (1.0f - transition.getProgress()) * (io.DisplaySize.y - windowPos.y);
    SetNextWindowPos(windowPos);
    SetNextWindowSize({fwidth, fheight});
    
    Begin("ApiKeyPrompt", nullptr, FLAGS);

    CustomTextInput("API Key", GLOBALS::scanner.config.apikey, 32);
    if (CustomTextButton("Save")) {
        auto &apiKey = GLOBALS::scanner.config.apikey;
        if (strlen(apiKey) == 32) {
            shouldClose = true;
            std::thread t([]() {
                GLOBALS::scanner.config.save();
                GLOBALS::scanner.config.fetchRequirements();
            });
            t.detach();
        }
    }

    End();

    PopStyleVar(2);
    PopStyleColor(1);
}