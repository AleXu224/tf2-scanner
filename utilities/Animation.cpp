#include "Animation.hpp"
#include "imgui.h"
#include "math.h"

Transition::Transition(float from, float to, float duration, TransitionType type) {
    this->from = from;
    this->to = to;
    this->duration = duration;
    this->progress = 0.0f;
    this->type = type;
}

void Transition::forward() {
    this->progress += ImGui::GetIO().DeltaTime * (1.0f / this->duration);
    if (this->progress > 1.0f) this->progress = 1.0f;
}

void Transition::backward() {
    this->progress -= ImGui::GetIO().DeltaTime * (1.0f / this->duration);
    if (this->progress < 0.0f) this->progress = 0.0f;
}

float BezierBlend(float t) {
    return t * t * (3.0f - 2.0f * t);
}

float easeInOutCubic(float t) {
    return t < 0.5 ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float easeInOutQuint(float t) {
    return t < 0.5 ? 16.0f * t * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

float Transition::getProgress() {
    if (type == TransitionType::bezier) return BezierBlend(this->progress);
    if (type == TransitionType::easeInOutCubic) return easeInOutCubic(this->progress);
    if (type == TransitionType::easeInOutQuint) return easeInOutQuint(this->progress);
    return this->progress;
}