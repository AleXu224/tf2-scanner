#include "Console.hpp"

#include "../globals.hpp"
#include "imgui.h"

using namespace ImGui;

static std::mutex consoleMutex;

void ConsoleWindow::draw() {
    if (!GLOBALS::scanner.showConsole) return;
    SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);
    Begin("Console", &GLOBALS::scanner.showConsole);
    GLOBALS::console.printOutput();
    End();
}

void Console::printOutput() {
    consoleMutex.lock();
    for (auto &outputMessage : this->output) {
        if (outputMessage.type == TYPES::STRING) {
            if (outputMessage.severity == SEVERITY::ERR) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(219.0f / 255, 67.0f / 255, 67.0f / 255, 1.0f));
            } else if (outputMessage.severity == SEVERITY::WARNING) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(235.0f / 255, 214.0f / 255, 116.0f / 255, 1.0f));
            }

            ImGui::Text("%s", outputMessage.message.c_str());

            if (outputMessage.severity != SEVERITY::INFO)
                ImGui::PopStyleColor();
        }
    }
    consoleMutex.unlock();
}

void Console::addOutput(std::string message, SEVERITY severity) {
    Output outputMessage(message, severity);
    consoleMutex.lock();
    this->output.push_back(outputMessage);
    consoleMutex.unlock();
}