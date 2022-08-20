#include "InfoCard.hpp"
#include "imgui.h"
#include "../globals.hpp"
#include "string"

using namespace ImGui;

static int instances = 0;
InfoCard::InfoCard(std::string message) : transition(0.0f, 1.0f, 0.2f, TransitionType::easeInOutCubic), timer(5.0f) {
    this->message = message;
    id = ++instances;
}

InfoCard::~InfoCard() {
    --instances;
}

void InfoCard::draw() {
    if (!shouldClose) transition.forward();
    else transition.backward();

    if (transition.getProgress() == 1.0f && !startedClosing) {
        timer.start();
        startedClosing = true;
    }
    else if (transition.getProgress() == 0.0f) canClose = true;

    if (timer.isDone() && startedClosing) shouldClose = true;

    PushFont(GLOBALS::FONTS[ROBOTO_16]);
    auto textSize = CalcTextSize(message.c_str());
    const float margin = 8.0f;
    const float padding = 16.0f;
    SetNextWindowSize({textSize.x + margin * 2, textSize.y + margin * 2});
    auto &io = GetIO();
    ImVec2 pos = {io.DisplaySize.x / 2 - textSize.x / 2, io.DisplaySize.y - (textSize.y + margin * 2 + padding) * transition.getProgress()};
    SetNextWindowPos(pos);
    PushStyleColor(ImGuiCol_WindowBg, COLORS::SECONDARY);
    PushStyleColor(ImGuiCol_Text, COLORS::PRIMARY);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(margin, margin));
    PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);

    std::stringstream ss;
    ss << "InfoCard###" << id;
    Begin(ss.str().c_str(), nullptr, FLAGS);
    Text(message.c_str());
    End();

    PopStyleVar(2);
    PopStyleColor(2);
    PopFont();
}