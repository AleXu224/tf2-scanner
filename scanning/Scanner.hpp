#ifndef BPSCANNER_SCANNER_HPP
#define BPSCANNER_SCANNER_HPP

#include "Config.hpp"

class Scanner {
   public:
    Config config;
    std::vector<Player> playerList;
    std::list<Player> playerPushList;

    bool showDrawer = false;
    bool isScanning = false;
    void Scan();
    void pushPlayers();
    
};

#endif  // BPSCANNER_SCANNER_HPP
