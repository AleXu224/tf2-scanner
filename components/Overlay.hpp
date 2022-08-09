#ifndef BPSCANNER_OVERLAY_HPP
#define BPSCANNER_OVERLAY_HPP

#include "functional"
#include "../utilities/Animation.hpp"

bool OverlayBackground(Transition& transition);
bool OverlayBackground();

class OverlayBase {
   public:
    bool shouldClose = false;
    bool canClose = false;

    virtual void draw() = 0;
};

class StatusIndicator : public OverlayBase {
   public:
    StatusIndicator() {
        canClose = true;
    }
    void draw();
};

class Overlay {
   private:
    static void cleanup();

   public:
    // static inline std::vector<OverlayComponent> components = {};
    static inline std::vector<OverlayBase *> overlays = {};
    // static void init();
    // static void addOverlay(std::function<void()> draw, OverlayBG bg = OverlayBG::Visible);
    static void addOverlay(OverlayBase *overlay);
    static void draw();
    static void pop();
};

#endif  // BPSCANNER_OVERLAY_HPP