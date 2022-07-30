#include "SideBar.hpp"

#include "../globals.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "string"
#include "thread"

using namespace ImGui;

bool CustomTextButton(const std::string &value) {
    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    // Invisible button and size calculation
    ImVec2 size = CalcTextSize(value.c_str(), nullptr, true);
    const int buttonWidth = GetWindowWidth() - 16 * 2;
    const int buttonHeight = 32;
    SetCursorPos({GetCursorPosX() + 16, GetCursorPosY() + 16});
    bool returnValue = InvisibleButton(value.c_str(), ImVec2(buttonWidth, buttonHeight));

    // Color calculation and transition
    ImGuiWindow *window = GetCurrentWindow();
    const ImGuiID id = window->GetID(value.c_str());
    static std::map<ImGuiID, float> buttons;
    ImGuiIO &io = ImGui::GetIO();
    ImVec4 buttonColor;
    ImVec4 textColor;
    if (IsItemHovered()) {
        buttons[id] += io.DeltaTime * (1.0f / 0.2f);
        if (buttons[id] > 1.0f) {
            buttons[id] = 1.0f;
        }
    } else {
        buttons[id]-= io.DeltaTime * (1.0f / 0.2f);
        if (buttons[id] < 0.0f) {
            buttons[id] = 0.0f;
        }
    }
    buttonColor = ImLerp(COLORS::PRIMARY_LIGHT, COLORS::SECONDARY, buttons[id]);
    textColor = ImLerp(COLORS::TEXT, COLORS::PRIMARY, buttons[id]);

    // Visible button and text
    ImDrawList *draw_list = GetWindowDrawList();
    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), ImColor(buttonColor), 4.0f);

    ImVec2 rectsize = GetItemRectSize();
    ImVec2 textPos = {
        GetItemRectMin().x + rectsize.x / 2 - size.x / 2,
        GetItemRectMin().y + rectsize.y / 2 - size.y / 2

    };
    draw_list->AddText(textPos, ImColor(textColor), value.c_str());
    PopFont();

    return returnValue;
}

void CustomIntInput(std::string input_name, int &input_value) {
    static std::map<std::string, bool> input_map;
    PushFont(GLOBALS::FONTS[ROBOTO_14]);
    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    PushStyleColor(ImGuiCol_Text, COLORS::TEXT);
    Text("%s", input_name.c_str());
    PopStyleColor();
    PopFont();

    PushStyleVar(ImGuiStyleVar_FramePadding, {4, 9});
    PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);

    PushStyleColor(ImGuiCol_FrameBg, COLORS::PRIMARY_LIGHT);

    SetCursorPosX(GetCursorPosX() + 16);
    SetNextItemWidth(GetWindowWidth() - 16 * 2);
    PushID(input_name.c_str());

    if (input_map[input_name])
        PushStyleColor(ImGuiCol_Border, COLORS::SECONDARY);
    else
        PushStyleColor(ImGuiCol_Border, COLORS::PRIMARY_LIGHT);

    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    InputInt("", &input_value, 0, 0);
    PopID();

    // It's gonna be one frame late but it's the best solution I could find
    if (IsItemActive())
        input_map[input_name] = true;
    else
        input_map[input_name] = false;

    PopStyleVar(3);
    PopStyleColor(2);
    PopFont();
}

void CustomTextInput(std::string input_name, char* input_value) {
    static std::map<std::string, bool> input_map;
    PushFont(GLOBALS::FONTS[ROBOTO_14]);
    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    PushStyleColor(ImGuiCol_Text, COLORS::TEXT);
    Text("%s", input_name.c_str());
    PopStyleColor();
    PopFont();

    PushStyleVar(ImGuiStyleVar_FramePadding, {4, 9});
    PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);

    PushStyleColor(ImGuiCol_FrameBg, COLORS::PRIMARY_LIGHT);

    SetCursorPosX(GetCursorPosX() + 16);
    SetNextItemWidth(GetWindowWidth() - 16 * 2);
    PushID(input_name.c_str());

    if (input_map[input_name])
        PushStyleColor(ImGuiCol_Border, COLORS::SECONDARY);
    else
        PushStyleColor(ImGuiCol_Border, COLORS::PRIMARY_LIGHT);

    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    InputText("", GLOBALS::scanner.config.apikey, 33);
    PopID();

    // It's gonna be one frame late but it's the best solution I could find
    if (IsItemActive())
        input_map[input_name] = true;
    else
        input_map[input_name] = false;

    PopStyleVar(3);
    PopStyleColor(2);
    PopFont();
}

void CustomFloatInput(std::string input_name, float &input_value) {
    static std::map<std::string, bool> input_map;
    PushFont(GLOBALS::FONTS[ROBOTO_14]);
    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    PushStyleColor(ImGuiCol_Text, COLORS::TEXT);
    Text("%s", input_name.c_str());
    PopStyleColor();
    PopFont();

    PushStyleVar(ImGuiStyleVar_FramePadding, {4, 9});
    PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);

    PushStyleColor(ImGuiCol_FrameBg, COLORS::PRIMARY_LIGHT);

    SetCursorPosX(GetCursorPosX() + 16);
    SetNextItemWidth(GetWindowWidth() - 16 * 2);
    PushID(input_name.c_str());

    if (input_map[input_name])
        PushStyleColor(ImGuiCol_Border, COLORS::SECONDARY);
    else
        PushStyleColor(ImGuiCol_Border, COLORS::PRIMARY_LIGHT);

    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    InputFloat("", &input_value, 0, 0, "%.2f");
    PopID();

    // It's gonna be one frame late but it's the best solution I could find
    if (IsItemActive())
        input_map[input_name] = true;
    else
        input_map[input_name] = false;

    PopStyleVar(3);
    PopStyleColor(2);
    PopFont();
}

void ScanTypeButton(ScanType buttonType, const float &buttonWidth, const float &height) {
    // TODO: add transition and roundedness
    ScanType &scanType = GLOBALS::scanner.scanType;
    ImDrawList *draw_list = GetWindowDrawList();

    InvisibleButton(std::to_string((int)buttonType).c_str(), {buttonWidth, height});
    ImColor buttonColor = scanType == buttonType ? ImColor(COLORS::SECONDARY) : ImColor(COLORS::PRIMARY_LIGHT);
    ImDrawCornerFlags corners = ImDrawCornerFlags_None;
    if (buttonType == ScanType::Steamids) corners = ImDrawCornerFlags_Left;
    if (buttonType == ScanType::Group) corners = ImDrawCornerFlags_Right;
    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), buttonColor, 4.0f, corners);
    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    auto textSize = CalcTextSize(ScanType_Strings.at(buttonType).c_str(), nullptr, true);
    ImVec2 textPos = {GetItemRectMin().x + (buttonWidth / 2) - (textSize.x / 2), GetItemRectMin().y + (height / 2) - (textSize.y / 2)};
    ImColor textColor = scanType == buttonType ? ImColor(COLORS::PRIMARY) : ImColor(COLORS::TEXT);
    draw_list->AddText(textPos, textColor, ScanType_Strings.at(buttonType).c_str());
    if (IsItemActive()) {
        scanType = buttonType;
    }
    PopFont();
}

void ScanTypeSelection() {
    auto *io = &ImGui::GetIO();
    const int padding = 16;
    const int height = 32;
    const int width = GetWindowSize().x - padding * 2;
    const int buttonCount = 2;
    const float buttonWidth = width / buttonCount;

    ImDrawList *draw_list = GetWindowDrawList();

    ScanType &scanType = GLOBALS::scanner.scanType;

    SetCursorPosX(GetCursorPosX() + padding);
    SetCursorPosY(GetCursorPosY() + padding);
    ScanTypeButton(ScanType::Steamids, buttonWidth, height);

    SameLine();
    ScanTypeButton(ScanType::Group, buttonWidth, height);
}

bool Tab(std::string tabName, bool tabActive) {
    const float tabHeight = 48;
    const int tabs = 2;
    const float tabWidth = GetWindowSize().x / tabs;
    auto ret = InvisibleButton(tabName.c_str(), {tabWidth, tabHeight});
    ImDrawList *draw_list = GetWindowDrawList();
    draw_list->AddRectFilled(GetItemRectMin(), GetItemRectMax(), ImColor(COLORS::PRIMARY));

    PushFont(GLOBALS::FONTS[ROBOTO_14]);
    ImVec2 textSize = CalcTextSize(tabName.c_str(), nullptr, true);
    ImVec2 textPos = {GetItemRectMin().x + (tabWidth / 2) - (textSize.x / 2), GetItemRectMin().y + (tabHeight / 2) - (textSize.y / 2)};
    draw_list->AddText(textPos, ImColor(COLORS::TEXT), tabName.c_str());
    PopFont();

    if (!tabActive) return ret;

    const float underlineHeight = 2;
    const float underlineWidth = tabWidth;
    ImVec2 underLinePos = {GetItemRectMin().x, GetItemRectMin().y + tabHeight - underlineHeight};
    draw_list->AddRectFilled(underLinePos, GetItemRectMax(), ImColor(COLORS::SECONDARY));
    return ret;
}

void TabsMenu(int &selectedTab) {
    if (Tab("Scanner", selectedTab == 0)) selectedTab = 0;
    SameLine();
    if (Tab("Settings", selectedTab == 1)) selectedTab = 1;
}

void CustomCheckbox(std::string name, bool &value) {
    const float height = 19;
    const float width = GetWindowSize().x - 16 * 2;
    PushFont(GLOBALS::FONTS[ROBOTO_16]);
    bool ret = InvisibleButton(name.c_str(), {width, height});
    ImDrawList *draw_list = GetWindowDrawList();
    ImVec2 textSize = CalcTextSize(name.c_str(), nullptr, true);
    ImVec2 textPos = {GetItemRectMin().x, GetItemRectMin().y + (height / 2) - (textSize.y / 2)};

    draw_list->AddText(textPos, ImColor(COLORS::TEXT), name.c_str());

    const float checkboxWidth = 16;
    const float checkboxHeight = 16;
    const float checkboxBorder = 2;

    ImVec2 checkboxPos = {GetItemRectMax().x - checkboxWidth, GetItemRectMin().y + (height - checkboxHeight) / 2};
    ImVec2 checkboxPos2 = {checkboxPos.x + checkboxWidth, checkboxPos.y + checkboxHeight};
    draw_list->AddRectFilled(checkboxPos, checkboxPos2, ImColor(COLORS::SECONDARY), 4.0f);

    if (ret) {
        value = !value;
    }

    if (!value) {
        ImVec2 checkboxPos3 = {checkboxPos.x + checkboxBorder, checkboxPos.y + checkboxBorder};
        ImVec2 checkboxPos4 = {checkboxPos2.x - checkboxBorder, checkboxPos2.y - checkboxBorder};
        draw_list->AddRectFilled(checkboxPos3, checkboxPos4, ImColor(COLORS::BACKGROUND), 2.0f);
    }

    PopFont();
}

void MainInput() {
    const float inputWidth = GetWindowSize().x - 16 * 2;
    float inputHeight = 200;
    if (GLOBALS::scanner.scanType != ScanType::Steamids) inputHeight = 32;

    ImGuiIO &io = ImGui::GetIO();

    static std::map<std::string, bool> input_map;
    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    PushFont(GLOBALS::FONTS[ROBOTO_14]);
    PushStyleColor(ImGuiCol_Text, COLORS::TEXT);
    if (GLOBALS::scanner.scanType == ScanType::Steamids) Text("Scan input");
    else if (GLOBALS::scanner.scanType == ScanType::Group) Text("Group link");
    else if (GLOBALS::scanner.scanType == ScanType::Friends) Text("Profile link");
    PopStyleColor();
    PopFont();

    PushStyleVar(ImGuiStyleVar_FramePadding, {4, 9});
    PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);

    PushStyleColor(ImGuiCol_FrameBg, COLORS::PRIMARY_LIGHT);

    SetCursorPosX(16);
    PushID("Main Input");

    static bool isActive = false;

    if (isActive)
        PushStyleColor(ImGuiCol_Border, COLORS::SECONDARY);
    else
        PushStyleColor(ImGuiCol_Border, COLORS::PRIMARY_LIGHT);

    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    ImGui::InputTextMultiline("", &GLOBALS::scanner.scanInput, {inputWidth, inputHeight});
    PopID();

    // It's gonna be one frame late but it's the best solution I could find
    if (IsItemActive())
        isActive = true;
    else
        isActive = false;

    PopStyleVar(3);
    PopStyleColor(2);
    PopFont();
}

void SideBarMenu() {
    const int SidebarWidth = 360;
    ImGuiIO &io = ImGui::GetIO();
    PushStyleColor(ImGuiCol_ChildBg, COLORS::PRIMARY);

    BeginChild("SideBar", ImVec2(SidebarWidth, GetWindowSize().y), false, FLAGS);
    static int selectedTab = 0;
    TabsMenu(selectedTab);

    if (selectedTab == 0) {
        ScanTypeSelection();

        MainInput();
        if (CustomTextButton("Start Scan")) {
            if (!GLOBALS::scanner.isScanning) {
                GLOBALS::scanner.showDrawer = false;
                std::thread t([]() {
                    GLOBALS::scanner.Scan();
                });
                t.detach();
            }
        }
        if (CustomTextButton("Clear Input")) {
            GLOBALS::scanner.scanInput.clear();
        }

        CustomFloatInput("Max Refined", GLOBALS::scanner.config.maxRef);
        CustomFloatInput("Max Keys", GLOBALS::scanner.config.maxKeys);
        CustomFloatInput("Min Refined", GLOBALS::scanner.config.minRef);
        CustomFloatInput("Min Keys", GLOBALS::scanner.config.minKeys);
        CustomIntInput("Max Hours", GLOBALS::scanner.config.maxHours);
        CustomIntInput("Max history", GLOBALS::scanner.config.maxHistory);

        SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
        CustomCheckbox("Untradable", GLOBALS::scanner.config.untradable);
        SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
        CustomCheckbox("No Value", GLOBALS::scanner.config.noValue);
        SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
        CustomCheckbox("Skins", GLOBALS::scanner.config.skins);

        SetCursorPosY(GetCursorPosY() + 16);
    } else if (selectedTab == 1) {
        CustomTextInput("API Key", GLOBALS::scanner.config.apikey);
        if (CustomTextButton("Save")) {
            GLOBALS::scanner.showDrawer = false;
            std::thread t([]() {
                GLOBALS::scanner.config.save();
            });
            t.detach();
        }
        if (CustomTextButton("Open Config Folder")) {
            std::string localAppData = std::getenv("LOCALAPPDATA");
            std::string path = localAppData + "\\BPScanner\\";
            std::string cmd = "explorer " + path;
            // system(cmd.c_str());
            ShellExecute(NULL, NULL, path.c_str(), NULL, NULL, SW_SHOW);
        }
        if (CustomTextButton("Force Update")) {
            GLOBALS::scanner.showDrawer = false;
            GLOBALS::scanner.config.lastUpdate = 0;
            std::thread t([]() {
                GLOBALS::scanner.config.fetchRequirements();
            });
            t.detach();
        }
    }

    EndChild();

    PopStyleColor();
}

void SideBar() {
    static float progress = 0.0f;
    ImGuiIO &io = ImGui::GetIO();
    if (GLOBALS::scanner.showDrawer) {
        progress += io.DeltaTime * (1.0f / 0.2f);
        if (progress > 1.0f) {
            progress = 1.0f;
        }
    } else {
        progress -= io.DeltaTime * (1.0f / 0.2f);
        if (progress < 0.0f) {
            progress = 0.0f;
        }
    }
    if (progress == 0.0f) return;
    const int SidebarWidth = 360;
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f * progress));
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    SetNextWindowPos(ImVec2(0, 0));

    Begin("SideBar", nullptr, (FLAGS | ImGuiWindowFlags_NoScrollWithMouse) - ImGuiWindowFlags_NoFocusOnAppearing);
    ImVec2 buttonSize = ImVec2(io.DisplaySize.x - SidebarWidth * BezierBlend(progress), io.DisplaySize.y);
    // This is so the program doesn't crash when the window is minimized
    if (buttonSize.x <= 0) buttonSize.x = 1;
    if (buttonSize.y <= 0) buttonSize.y = 1;
    if (InvisibleButton("SideBarBg", buttonSize)) {
        GLOBALS::scanner.showDrawer = false;
    }
    SameLine();
    SideBarMenu();
    End();

    PopStyleColor(1);
    PopStyleVar(2);
}