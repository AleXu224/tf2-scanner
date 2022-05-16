#include <string>
#include <map>
#include <iostream>
#include "MainBody.hpp"
#include "imgui.h"
#include "../globals.hpp"
#include "imgui_internal.h"
#include "../fonts/IconsMaterialDesign.h"

using namespace ImGui;

void UserAvatar() {
    SetCursorPos({GetCursorPosX() + 16, GetCursorPosY() + 16});
    InvisibleButton("UserAvatar", ImVec2(64, 64));
    ImDrawList *draw_list = GetWindowDrawList();

    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), GetColorU32(COLORS::SECONDARY), 16.0f);
}

struct ButtonStatus {
    double progress = 0.0f; // 0.0 - 1.0
};

bool UserButton(const std::string &value) {
    // Invisible button and size calculation
    ImVec2 size = CalcTextSize(value.c_str(), nullptr, true);
    bool returnValue = InvisibleButton(value.c_str(), ImVec2(32, 32));

    // Color calculation and transition
    ImGuiWindow *window = GetCurrentWindow();
    const ImGuiID id = window->GetID(value.c_str());
    static std::map<ImGuiID, ButtonStatus> buttons;
    ImGuiIO &io = ImGui::GetIO();
    ImVec4 buttonColor;
    ImVec4 textColor;
    if (IsItemHovered()) {
        buttons[id].progress += io.DeltaTime * (1.0f / 0.2f);
        if (buttons[id].progress > 1.0f) {
            buttons[id].progress = 1.0f;
        }
    } else {
        buttons[id].progress -= io.DeltaTime * (1.0f / 0.2f);
        if (buttons[id].progress < 0.0f) {
            buttons[id].progress = 0.0f;
        }
    }
    buttonColor = ImLerp(COLORS::PRIMARY_LIGHT, COLORS::SECONDARY, buttons[id].progress);
    textColor = ImLerp(COLORS::TEXT, COLORS::PRIMARY, buttons[id].progress);


    // Visible button and text
    ImDrawList *draw_list = GetWindowDrawList();
    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), ImColor(buttonColor), 4.0f);

    ImVec2 rectsize = GetItemRectSize();
    ImVec2 textPos = {
            GetItemRectMin().x + rectsize.x / 2 - size.x / 2,
            // magic numbers here, just ignore
            GetItemRectMin().y + 24.0f / 2

    };
    draw_list->AddText(textPos, ImColor(textColor), value.c_str());

    return returnValue;
}

void UserButtons(int &userid) {
    PushFont(GLOBALS::FONTS[ICONS]);
    SameLine();
    SetCursorPos({GetCursorPosX() + 16, GetCursorPosY() + 16});
    if (UserButton(ICON_MD_ADD)) {
        std::cout << userid << std::endl;
    }
    SameLine();
    SetCursorPosX(GetCursorPosX() + 8);
    SetCursorPosY(GetCursorPosY() + 16);
    UserButton(ICON_MD_INVENTORY_2);
    SameLine();
    PopFont();
}

void Item() {
    InvisibleButton("Item", ImVec2(64, 64));
    ImDrawList *draw_list = GetWindowDrawList();
    const ImVec2 secondary_pos_max = {GetItemRectMax().x, GetItemRectMax().y - 48};
    draw_list->AddRectFilled(GetItemRectMin(), secondary_pos_max, GetColorU32(COLORS::BACKGROUND), 8.0f, ImDrawFlags_RoundCornersTop);

    const ImVec2 main_pos_max = {GetItemRectMax().x , GetItemRectMax().y - 16};
    const ImVec2 main_pos_min = {GetItemRectMin().x, GetItemRectMin().y + 8};
    draw_list->AddRectFilled(main_pos_min, main_pos_max, GetColorU32(COLORS::SECONDARY), 8.0f, ImDrawFlags_RoundCornersTop);

    const ImVec2 bottomPartPosition = {GetItemRectMin().x, GetItemRectMin().y + 48};
    draw_list->AddRectFilled(bottomPartPosition, GetItemRectMax(), GetColorU32(COLORS::BACKGROUND), 8.0f, ImDrawFlags_RoundCornersBottom);
}

void UserItems() {
    ImGuiIO &io = ImGui::GetIO();
    SameLine();
    SetCursorPos({GetCursorPosX() + 8, GetCursorPosY()});
    //                    width - space before items - scrollbar
    const int maxItems = (io.DisplaySize.x - 176 - 14 - 64) / (64 + 8);
    for (int i = 0; i < maxItems; i++) {
        if (i != 0) {
            SameLine();
        }
        SetCursorPosX(GetCursorPosX() + 8);
        Item();
    }
}

void UserTag(const std::string &tag) {
    const ImVec2 textSize = CalcTextSize(tag.c_str(), nullptr, true);
    InvisibleButton("UserTag", ImVec2(textSize.x + 8 * 2, 24));
    ImDrawList *draw_list = GetWindowDrawList();
    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), GetColorU32(COLORS::PRIMARY_LIGHT), 4.0f);

    const ImVec2 textPos = {GetItemRectMin().x + 8, GetItemRectMin().y + (24 - textSize.y) / 2};
    draw_list->AddText(textPos, GetColorU32(COLORS::TEXT), tag.c_str());
}

void UserTags() {
    SetCursorPos({GetWindowPos().x + 8, GetWindowPos().x + 104});

    for (int i = 0; i < 5; i++) {
        if (i != 0) {
            SameLine();
        }
        SetCursorPosX(GetCursorPosX() + 8);
        const std::string tag = "Tag " + std::to_string(i);
        UserTag(tag);
    }
}

void UserChild(int windowID, int &userID) {
    ImGuiIO &io = GetIO();

    // top elements
    BeginChild(windowID + 1, ImVec2(GetWindowSize().x, 136), false, FLAGS);
    UserAvatar();
    UserButtons(userID);
    UserItems();
    SameLine();
    SetCursorPos({GetWindowSize().x - 14 - 16 - 32, GetCursorPosY() + 16});

    PushFont(GLOBALS::FONTS[ICONS]);
    UserButton(ICON_MD_CLOSE);
    PopFont();

    // bottom elements
    UserTags();

    EndChild();
}

void MainBody() {
    ImGuiIO &io = GetIO();
    SetNextWindowPos(ImVec2(0, 48));
    SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - 48));

    PushStyleColor(ImGuiCol_WindowBg, COLORS::BACKGROUND);
    PushStyleColor(ImGuiCol_ChildBg, COLORS::PRIMARY);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 8));

    Begin("Main Body", nullptr, FLAGS);
    SetCursorPosY(GetCursorPosY() + 8);

    // Skip the users that are not visible by creating a padding above the first visible user
    // This improves the fps from 67 to nearly 3k with 11k users
    int childheight = 136 + 8;
    int skipChildren = GetScrollY() / childheight;
    int stop = GetWindowSize().y / childheight + 1 + skipChildren;
    SetCursorPosY(GetCursorPosY() + skipChildren * childheight);

    int listSize = 100;
    for (int i = skipChildren; i < listSize && i <= stop; i++) {
        // We repurpose the IDs so that only the minimum amount of windows are created
        // Creating a window for every user would slow the frame time by a lot
        UserChild(i - skipChildren, i);
    }

    // Skip the users that are not visible below
    SetCursorPosY(GetCursorPosY() + (listSize - stop - 1) * childheight);
    End();

    PopStyleColor(2);
    PopStyleVar(2);
}