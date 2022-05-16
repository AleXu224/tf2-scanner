//
// Created by Squizell on 15/05/2022.
//

#ifndef BPSCANNER_CONSOLE_HPP
#define BPSCANNER_CONSOLE_HPP

#include <string>
#include <vector>
#include <imgui.h>

struct TYPES {
    static constexpr int STRING = 0;
};

struct SEVERITY {
    static constexpr int INFO = 0;
    static constexpr int WARNING = 1;
    static constexpr int ERR = 2;
};

class ConsoleOutput {
public:
    int type = 0;
    int severity = 0;
    std::string message;
};

class Console {
public:
    std::vector<ConsoleOutput> output;

    void addOutput(std::string message, int severity = 0) {
        ConsoleOutput outputMessage;
        outputMessage.type = TYPES::STRING;
        outputMessage.severity = severity;
        outputMessage.message = message;
        this->output.push_back(outputMessage);
    }

    void printOutput() {
        for (auto &outputMessage : this->output) {
            if (outputMessage.type == TYPES::STRING) {
                if (outputMessage.severity == SEVERITY::ERR) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(219.0f/255, 67.0f/255, 67.0f/255, 1.0f));
                } else if (outputMessage.severity == SEVERITY::WARNING) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(235.0f/255, 214.0f/255, 116.0f/255, 1.0f));
                }

                ImGui::Text(outputMessage.message.c_str());

                if (outputMessage.severity != SEVERITY::INFO) ImGui::PopStyleColor();
            }
        }
    }
};

void ConsoleWindow(bool &showConsole);


#endif //BPSCANNER_CONSOLE_HPP
