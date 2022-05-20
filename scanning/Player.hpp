// create header guards
#ifndef BPSCANNER_PLAYER_HPP
#define BPSCANNER_PLAYER_HPP

#include "string"
#include "Inventory.hpp"
#include "../json_schemas/PlayerData.hpp"
#include "imgui.h"

class Player {
public:
    std::string steamid;
    std::string name;
    std::string avatarUrl;
    int visibility;
    int level{};
    int histories{};
    int hours{};
    Inventory inventory;

    Player(JsonPlayer::Player &player);

    void ToConsole();
};

#endif //BPSCANNER_PLAYER_HPP

