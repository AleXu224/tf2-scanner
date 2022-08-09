#ifndef RAYLIBTESTING_STATSINFO_HPP
#define RAYLIBTESTING_STATSINFO_HPP

#include "Overlay.hpp"

class StatsInfo : public OverlayBase {
    public:
    StatsInfo() {
        canClose = true;
    }
    void draw();
};

#endif  // RAYLIBTESTING_STATSINFO_HPP
