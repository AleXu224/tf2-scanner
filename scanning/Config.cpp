//
// Created by Squizell on 15/05/2022.
//

#include "Config.hpp"
#include "../globals.hpp"

void Config::consoleLog(std::string message, int severity) {
    GLOBALS::console.addOutput(message, severity);
}