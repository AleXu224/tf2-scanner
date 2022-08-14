#pragma once

#include "./Overlay.hpp"
#include "../utilities/Animation.hpp"

class InfoCard : public OverlayBase {
    private:
    Transition transition;
    Timer timer;
    std::string message;
    bool startedClosing = false;
    int id;

    public:
    InfoCard(std::string message);
    void draw();

    ~InfoCard();
};

