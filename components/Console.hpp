#ifndef BPSCANNER_CONSOLE_HPP
#define BPSCANNER_CONSOLE_HPP

#include <imgui.h>

#include "../scanning/Inventory.hpp"
#include "../scanning/Item.hpp"
#include "../scanning/Player.hpp"
#include "../components/Overlay.hpp"
#include <iostream>
#include <string>
#include <vector>

enum class TYPES {
    STRING = 0,
    PLAYER = 1,
    INVENTORY = 2,
    ITEM = 3
};

enum class SEVERITY {
    INFO = 0,
    WARNING = 1,
    ERR = 2
};

class Output {
   public:
    TYPES type = TYPES::STRING;
    SEVERITY severity = SEVERITY::INFO;
    std::string message;
    int index;

    Output(std::string message, SEVERITY severity) {
        this->message = message;
        this->severity = severity;
    }

    Output(int arrayPos, TYPES type) {
        this->type = type;
        this->index = arrayPos;
    }
};

class Console {
   public:
    std::vector<Output> output;
    std::vector<Player> playerList;
    std::vector<Inventory> inventoryList;
    std::vector<Item> itemList;

    void addOutput(std::string message, SEVERITY severity = SEVERITY::INFO);
    void addOutput(Player &player);
    void addOutput(Inventory &inventory);
    void addOutput(Item &item);

    // void addOutput(Inventory inventory, int severity = 0);
    // void addOutput(Item item, int severity = 0);

    void printOutput();
};

class ConsoleWindow : public OverlayBase {
   public:
    ConsoleWindow() {
        canClose = true;
    }
    void draw();
};

#endif  // BPSCANNER_CONSOLE_HPP
