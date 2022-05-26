#include "MainBody.hpp"

#include <iostream>
#include <map>
#include <string>

#include "../fonts/IconsMaterialDesign.h"
#include "../globals.hpp"
#include "../utilities/Texture.hpp"
#include "imgui.h"
#include "imgui_internal.h"

using namespace ImGui;

#define players GLOBALS::scanner.playerList

bool UserAvatar(int &playerIndex) {
    SetCursorPos({GetCursorPosX() + 16, GetCursorPosY() + 16});
    auto ret = InvisibleButton("UserAvatar", ImVec2(64, 64));
    ImDrawList *draw_list = GetWindowDrawList();

    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), GetColorU32(COLORS::SECONDARY), 16.0f);
    draw_list->AddImageRounded((void *)(intptr_t)Texture::getFromUrl(players[playerIndex].avatarUrl).id, GetItemRectMin(), GetItemRectMax(), {0, 0}, {1, 1}, ImColor(255, 255, 255, 255), 8.0f);
    return ret;   
}

struct ButtonStatus {
    double progress = 0.0f;  // 0.0 - 1.0
};

bool TextButton(const std::string &value) {
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

void UserButtons(int &playerIndex) {
    PushFont(GLOBALS::FONTS[ICONS]);
    SameLine();
    SetCursorPos({GetCursorPosX() + 16, GetCursorPosY() + 16});
    if (TextButton(ICON_MD_ADD)) {
    }
    SameLine();
    SetCursorPosX(GetCursorPosX() + 8);
    SetCursorPosY(GetCursorPosY() + 16);
    if (TextButton(ICON_MD_INVENTORY_2)) {
        ShellExecute(0, 0, (std::string("https://backpack.tf/profiles/") + players[playerIndex].steamid).c_str(), 0, 0, SW_SHOW);
    }
    SameLine();
    PopFont();
}

void UserItem(int &playerIndex, int &itemIndex) {
    InvisibleButton("Item", ImVec2(64, 64));
    ImDrawList *draw_list = GetWindowDrawList();
    const ImVec2 secondary_pos_max = {GetItemRectMax().x, GetItemRectMax().y - 48};
    #define item players[playerIndex].inventory.items[itemIndex] 
    ImColor secondaryColor = item.qualitySecondary == QUALITY::NONE ? QUALITY_COLORS.at(item.quality) : QUALITY_COLORS.at(item.qualitySecondary);
    draw_list->AddRectFilled(GetItemRectMin(), secondary_pos_max, secondaryColor, 8.0f, ImDrawFlags_RoundCornersTop);

    const ImVec2 main_pos_max = {GetItemRectMax().x, GetItemRectMax().y - 16};
    const ImVec2 main_pos_min = {GetItemRectMin().x, GetItemRectMin().y + 8};
    draw_list->AddRectFilled(main_pos_min, main_pos_max, QUALITY_COLORS.at(item.quality), 8.0f, ImDrawFlags_RoundCornersTop);
    
    if (item.effectID != -1)
        draw_list->AddImage((void *)(intptr_t)Texture::getFromUrl(effectUrlPrefix + std::to_string(item.effectID) + effecturlSuffix, true).id, GetItemRectMin(), GetItemRectMax());
    draw_list->AddImage((void *)(intptr_t)Texture::getFromUrl(imageUrlPrefix + item.imageUrl, true).id, GetItemRectMin(), GetItemRectMax());

    PushFont(GLOBALS::FONTS[ROBOTO_10]);
    const ImVec2 bottomPartPosition = {GetItemRectMin().x, GetItemRectMin().y + 48};
    draw_list->AddRectFilled(bottomPartPosition, GetItemRectMax(), GetColorU32(COLORS::BACKGROUND), 8.0f, ImDrawFlags_RoundCornersBottom);
    std::stringstream priceString;

    int precision = 0;
    if (remainder(item.price, 1) != 0) precision = 2;
    priceString << std::fixed << std::setprecision(precision) << item.price << " " << TF2CURRENCY_STRINGS.at(item.currency);

    ImVec2 priceSize = CalcTextSize(priceString.str().c_str(), nullptr, true);
    ImVec2 pricePos = {
        GetItemRectMin().x + 32 - priceSize.x / 2,
        GetItemRectMin().y + 56 - priceSize.y / 2};
    draw_list->AddText(pricePos, GetColorU32(COLORS::TEXT), priceString.str().c_str());
    PopFont();

    if (IsItemHovered()) {
        ImGui::SetTooltip("%s", item.name.c_str());
    }
}

void UserItems(int &playerIndex) {
    ImGuiIO &io = ImGui::GetIO();
    SameLine();
    SetCursorPos({GetCursorPosX() + 8, GetCursorPosY()});
    //                    width - space before items - scrollbar
    const int maxItems = min((io.DisplaySize.x - 176 - 14 - 64) / (64 + 8), players[playerIndex].inventory.items.size());
    for (int i = 0; i < maxItems; i++) {
        if (i != 0) {
            SameLine();
        }
        SetCursorPosX(GetCursorPosX() + 8);
        UserItem(playerIndex, i);
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

void UserTags(int &playerIndex) {
    SetCursorPos({GetWindowPos().x + 8, GetWindowPos().x + 104});

    // for (int i = 0; i < 5; i++) {
    //     if (i != 0) {
    //         SameLine();
    //     }
    //     SetCursorPosX(GetCursorPosX() + 8);
    //     const std::string tag = "Tag " + std::to_string(i);
    //     UserTag(tag);
    // }
    std::stringstream refInInv;
    refInInv << std::fixed << std::setprecision(2) << players[playerIndex].inventory.scrapCount / 9.0f << " Ref";
    SetCursorPosX(GetCursorPosX() + 8);
    UserTag(refInInv.str());

    SameLine();
    SetCursorPosX(GetCursorPosX() + 8);
    std::stringstream keysInInv;
    keysInInv << players[playerIndex].inventory.keysCount << " Keys";
    UserTag(keysInInv.str());
}

void UserChild(int windowID, int &playerIndex) {
    ImGuiIO &io = GetIO();

    // top elements
    BeginChild(windowID + 1, ImVec2(GetWindowSize().x, 136), false, FLAGS);
    if (UserAvatar(playerIndex)) {
        ShellExecute(0, 0, (std::string("http://steamcommunity.com/profiles/") + players[playerIndex].steamid).c_str(), 0, 0, SW_SHOW);
    }
    UserButtons(playerIndex);
    UserItems(playerIndex);
    SameLine();
    SetCursorPos({GetWindowSize().x - 14 - 16 - 32, GetCursorPosY() + 16});

    PushFont(GLOBALS::FONTS[ICONS]);
    TextButton(ICON_MD_CLOSE);
    PopFont();

    // bottom elements
    UserTags(playerIndex);

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

    // Pushes the user queue into the player vector before doing anything
    // turns out vector.push_back is not thread safe (:
    GLOBALS::scanner.pushPlayers();

    const int listSize = players.size();
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