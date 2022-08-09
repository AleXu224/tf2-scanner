#ifndef BPSCANNER_LOADINGSCREEN_HPP
#define BPSCANNER_LOADINGSCREEN_HPP

#include "Overlay.hpp"

class LoadingScreen : public OverlayBase {
    Transition transition;
    public:
    LoadingScreen() : transition(0.0f, 1.0f, 0.2f, TransitionType::easeInOutCubic) {
        canClose = false;
    }
    void draw();
};

#endif //BPSCANNER_LOADINGSCREEN_HPP