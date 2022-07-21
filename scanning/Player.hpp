#ifndef BPSCANNER_PLAYER_HPP
#define BPSCANNER_PLAYER_HPP

#include "../json_schemas/PlayerData.hpp"
#include "Inventory.hpp"
#include "imgui.h"
#include "string"

class Player {
   public:
    std::string steamid;
    std::string name;
    std::string avatarUrl;
    int visibility;
    int level = -1;
    int histories = -1;
    int hours = -1;
    Inventory inventory;

    Player(JsonPlayer::Player &player);
    Player(std::string steamid);

    void getLevel();
    void getHistories();
    void getHours();

    void ToConsole();
};

#endif  // BPSCANNER_PLAYER_HPP
