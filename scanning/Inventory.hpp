#ifndef BPSCANNNER_INVENTORY_HPP
#define BPSCANNNER_INVENTORY_HPP

#include "Item.hpp"
#include "Player.hpp"
#include "vector"

class Player;

class Inventory {
public:
    std::vector<Item> items;
    int scrapCount = 0;
    int keysCount = 0;
    bool success = false;
    Player *player;

    Inventory (Player *player);

    void GetInventory();

    void ToConsole();
};


#endif //BPSCANNNER_INVENTORY_HPP