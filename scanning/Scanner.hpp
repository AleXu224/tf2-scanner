#ifndef BPSCANNER_SCANNER_HPP
#define BPSCANNER_SCANNER_HPP

#include "Config.hpp"

enum class ScanType {
    Steamids = 0,
    Group = 1,
    Friends = 2  
};

const std::map<ScanType, std::string> ScanType_Strings = {
    {ScanType::Steamids, "Steamids"},
    {ScanType::Group, "Group"},
    {ScanType::Friends, "Friends"}
};

class Scanner {
   public:
    Config config;
    std::vector<Player> playerList;
    std::list<Player> playerPushList;
    std::list<int> playerPopList;
    std::string scanInput;
    bool showDrawer = false;
    bool isScanning = false;
    bool showStats = false;
    bool showConsole = false;
    bool stopScanning = false;
    bool updateAvailable = false;
    std::string updateVersion = "";
    std::string updateLink = "";
    ScanType scanType = ScanType::Steamids;
    std::string scanStatus;
    void Scan();
    void pushPlayers();
    void popPlayers();
    
    Scanner() : config(){}
};

#endif  // BPSCANNER_SCANNER_HPP
