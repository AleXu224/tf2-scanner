#pragma once

#include "chrono"

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

class Timer {
    private:
    std::chrono::high_resolution_clock::time_point startTime;
    float duration;

    public:
    Timer(float duration);
    void start();
    bool isDone();
};