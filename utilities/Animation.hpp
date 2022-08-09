#pragma once

enum class TransitionType {
    linear,
    bezier,
    easeInOutCubic,
    easeInOutQuint
};

class Transition {
    private:
    float from;
    float to;
    float duration;
    float progress;
    TransitionType type;

    public:
    Transition (float from, float to, float duration, TransitionType type = TransitionType::linear);
    void forward();
    void backward();
    float getProgress();
};