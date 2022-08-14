#include "SideBar.hpp"

#include "../globals.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "../utilities/Animation.hpp"
#include "string"
#include "thread"

using namespace ImGui;

void CustomHelpMarker(std::string text) {
    SameLine();
    TextDisabled("(?)");
    if (IsItemHovered()) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_PopupBg, COLORS::PRIMARY_LIGHT);
        ImGui::PushStyleColor(ImGuiCol_Text, COLORS::TEXT);
        ImGui::SetTooltip("%s", text.c_str());
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(3);
    }
}

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

void CustomInput::draw() {
    PushFont(GLOBALS::FONTS[ROBOTO_14]);
    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    PushStyleColor(ImGuiCol_Text, COLORS::TEXT);
    Text("%s", this->inputName.c_str());
    if (!this->toolTip.empty()) CustomHelpMarker(this->toolTip);
    PopStyleColor();
    PopFont();

    PushStyleVar(ImGuiStyleVar_FramePadding, {4, 9});
    PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

    PushStyleColor(ImGuiCol_FrameBg, COLORS::PRIMARY_LIGHT);

    SetCursorPosX(GetCursorPosX() + 16);
    SetNextItemWidth(GetWindowWidth() - 16 * 2);
    PushID(this->inputName.c_str());

    if (this->isActive) transition.forward();
    else transition.backward();

    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f * transition.getProgress());
    PushStyleColor(ImGuiCol_Border, ImLerp(COLORS::PRIMARY_LIGHT, COLORS::SECONDARY, transition.getProgress()));

    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    if (inputType == InputType::INT) InputInt("", this->inputValueInt.value(), 0, 0);
    if (inputType == InputType::FLOAT) InputFloat("", this->inputValueFloat.value(), 0, 0, "%.2f");
    if (inputType == InputType::TEXT) InputText("", this->inputValueText.value(), maxSize + 1);
    if (inputType == InputType::TEXTMULTILINE) InputTextMultiline("", this->inputValueTextMultiline.value(), {width, height});
    PopID();

    // It's gonna be one frame late but it's the best solution I could find
    if (IsItemActive())
        this->isActive = true;
    else
        this->isActive = false;

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

void SideBar::draw() {
    if (GLOBALS::scanner.showDrawer) {
        transition.forward();
    } else {
        transition.backward();
    }

    if (transition.getProgress() == 0.0f) return;
    if (OverlayBackground(transition)) GLOBALS::scanner.showDrawer = false;

    const int SidebarWidth = 360;
    ImGuiIO &io = ImGui::GetIO();
    PushStyleColor(ImGuiCol_WindowBg, COLORS::PRIMARY);
    PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
    PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.0f, 0.0f});

    SetNextWindowPos({io.DisplaySize.x - SidebarWidth * transition.getProgress(), 0});
    SetNextWindowSize({SidebarWidth, io.DisplaySize.y});

    Begin("SideBar", nullptr, FLAGS);
    static int selectedTab = 0;
    TabsMenu(selectedTab);

    if (selectedTab == 0) {
        ScanTypeSelection();

        static auto mainInput = CustomInput("Scan Input", GLOBALS::scanner.scanInput, GetWindowSize().x - 16 * 2, 200);
        static auto mainInputGroup = CustomInput("Group Link", GLOBALS::scanner.scanInput, GetWindowSize().x - 16 * 2, 32);
        if (GLOBALS::scanner.scanType == ScanType::Steamids) mainInput.draw();
        else if (GLOBALS::scanner.scanType == ScanType::Group) mainInputGroup.draw();

        if (GLOBALS::scanner.scanType == ScanType::Group) {
            static auto skipPagesInput = CustomInput("Skip pages", GLOBALS::scanner.config.groupSkipPages, "The number of thousands of players the scanner will skip in the group, 1 page = 1000 players");
            static auto scanPagesInput = CustomInput("Scan pages", GLOBALS::scanner.config.groupScanPages, "The number of thousands of players the scanner will scan in the group.\nLimited to 30 since after that you get a timeout from steam");
            skipPagesInput.draw();
            scanPagesInput.draw();
            if (GLOBALS::scanner.config.groupScanPages > 30) GLOBALS::scanner.config.groupScanPages = 30;
        }
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

        static auto nameFilterInput = CustomInput("Name filter", GLOBALS::scanner.config.nameFilter, 64, "Name filter for items (case sensitive). Surround with / for regex.");
        nameFilterInput.draw();

        if (strlen(GLOBALS::scanner.config.nameFilter) > 0) {
            if (CustomTextButton("Clear Name Filter")) {
                GLOBALS::scanner.config.nameFilter[0] = '\0';
            }
        }

        static auto maxRefInput = CustomInput("Max Refined", GLOBALS::scanner.config.maxRef, "Max amount of pure currency allowed in the user's inventory (this includes keys)");
        static auto maxKeysInput = CustomInput("Max Keys", GLOBALS::scanner.config.maxKeys, "Max amount of pure currency allowed in the user's inventory (this includes refined)");
        static auto minRefInput = CustomInput("Min Refined", GLOBALS::scanner.config.minRef, "Minimum item price allowed for an item to be displayed");
        static auto minKeysInput = CustomInput("Min Keys", GLOBALS::scanner.config.minKeys, "Minimum item price allowed for an item to be displayed");
        static auto maxHoursInput = CustomInput("Max Hours", GLOBALS::scanner.config.maxHours, "Maximum number of hours a user can have before being skipped");
        static auto maxHistoryInput = CustomInput("Max history", GLOBALS::scanner.config.maxHistory, "Maximum number of refreshes a user can have on backpack.tf before being skipped");
        maxRefInput.draw();
        maxKeysInput.draw();
        minRefInput.draw();
        minKeysInput.draw();
        maxHoursInput.draw();
        maxHistoryInput.draw();

        SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
        CustomCheckbox("Untradable", GLOBALS::scanner.config.untradable);
        SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
        CustomCheckbox("No Value", GLOBALS::scanner.config.noValue);
        SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
        CustomCheckbox("Skins", GLOBALS::scanner.config.skins);

        SetCursorPosY(GetCursorPosY() + 16);
    } else if (selectedTab == 1) {
        static auto apiKeyInput = CustomInput("API Key", GLOBALS::scanner.config.apikey, 32);
        apiKeyInput.draw();

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

    End();

    PopStyleColor();
    PopStyleVar(2);
}
