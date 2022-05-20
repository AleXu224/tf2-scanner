#ifndef BPSCANNER_SCANNER_HPP
#define BPSCANNER_SCANNER_HPP


#include "Config.hpp"

class Scanner {
public:
    Config config;
    std::vector<Player> playerList;

    bool showDrawer = false;
    bool isScanning = false;
    void Scan();
};


#endif //BPSCANNER_SCANNER_HPP
