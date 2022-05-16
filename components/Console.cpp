//
// Created by Squizell on 15/05/2022.
//

#include "Console.hpp"
#include "../globals.hpp"
#include "imgui.h"

using namespace ImGui;

void ConsoleWindow(bool &showConsole) {
    if (!showConsole) return;
    SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);
    Begin("Console", &showConsole);
    GLOBALS::console.printOutput();
    End();
}