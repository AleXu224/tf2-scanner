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

void Console::printOutput() {
    for (auto &outputMessage : this->output) {
        if (outputMessage.type == TYPES::STRING) {
            if (outputMessage.severity == SEVERITY::ERR) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(219.0f / 255, 67.0f / 255, 67.0f / 255, 1.0f));
            } else if (outputMessage.severity == SEVERITY::WARNING) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(235.0f / 255, 214.0f / 255, 116.0f / 255, 1.0f));
            }

            ImGui::Text(outputMessage.message.c_str());

            if (outputMessage.severity != SEVERITY::INFO)
                ImGui::PopStyleColor();
        } else if (outputMessage.type == TYPES::PLAYER) {
            playerList[outputMessage.index].ToConsole();
        } else if (outputMessage.type == TYPES::INVENTORY) {
            inventoryList[outputMessage.index].ToConsole();
        } else if (outputMessage.type == TYPES::ITEM) {
            itemList[outputMessage.index].ToConsole();
        }
    }
}

void Console::addOutput(std::string message, SEVERITY severity) {
    Output outputMessage(message, severity);
    this->output.push_back(outputMessage);
}

void Console::addOutput(Player &player) {
    playerList.push_back(player);
    Output outputMessage(playerList.size() - 1, TYPES::PLAYER);
    this->output.push_back(outputMessage);
}

void Console::addOutput(Inventory &inventory) {
    inventoryList.push_back(inventory);
    Output outputMessage(inventoryList.size() - 1, TYPES::INVENTORY);
    this->output.push_back(outputMessage);
}

void Console::addOutput(Item &item) {
    itemList.push_back(item);
    Output outputMessage(itemList.size() - 1, TYPES::ITEM);
    this->output.push_back(outputMessage);
}
