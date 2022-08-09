#ifndef BPSCANNER_APIKEYPROMPT_HPP
#define BPSCANNER_APIKEYPROMPT_HPP

#include "Overlay.hpp"

class ApiKeyPrompt : public OverlayBase {
    Transition transition;
    public:
    ApiKeyPrompt() : transition(0.0f, 1.0f, 0.2f, TransitionType::easeInOutCubic) {
        canClose = false;
    }
    void draw();
};

#endif //BPSCANNER_APIKEYPROMPT_HPP