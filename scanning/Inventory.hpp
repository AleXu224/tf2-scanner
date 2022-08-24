#ifndef BPSCANNNER_INVENTORY_HPP
#define BPSCANNNER_INVENTORY_HPP

#include "Item.hpp"
#include "vector"

class Inventory {
   private:
    char search[32] = {0};

   public:
    std::vector<Item> items;
    int scrapCount = 0;
    int keysCount = 0;
    bool success = false;
    std::string steamid;

    bool steamErr = false;

    Inventory(std::string steamid);

    void GetInventory();

    float getCurrencyInInventory();

    void ToConsole();
};

#endif  // BPSCANNNER_INVENTORY_HPP