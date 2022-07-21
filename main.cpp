#include "components/ApiKeyPrompt.hpp"
#include "components/LoadingScreen.hpp"
#include "components/MainBody.hpp"
#include "components/SideBar.hpp"
#include "components/StatsInfo.hpp"
#include "components/TopBar.hpp"
#include "components/Overlay.hpp"
#include "cpr/cpr.h"
#include "fonts/IconsMaterialDesign.h"
#include "fonts/MaterialIcons.cpp"
#include "fonts/Roboto.cpp"
#include "globals.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include "GLFW/glfw3.h"
#include "utilities/Texture.hpp"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int, char**) {
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(800, 600, "wew", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    GLOBALS::FONTS.push_back(io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 14.0f));
    GLOBALS::FONTS.push_back(io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 12.0f));
    GLOBALS::FONTS.push_back(io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 19.0f));
    GLOBALS::FONTS.push_back(io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 17.0f));

    ImVector<ImWchar> icons_ranges;
    ImFontGlyphRangesBuilder icons_builder;
    icons_builder.AddChar(0xe145);  // add icon
    icons_builder.AddChar(0xe1a1);  // inventory icon
    icons_builder.AddChar(0xe5cd);  // close icon
    icons_builder.BuildRanges(&icons_ranges);
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    GLOBALS::FONTS.push_back(io.Fonts->AddFontFromMemoryCompressedTTF(MaterialIcons_compressed_data, MaterialIcons_compressed_size, 24.0f, &icons_config, icons_ranges.Data));

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 0.0f;
    style.ScrollbarSize = 3.0f;
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, COLORS::BACKGROUND);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, COLORS::SECONDARY);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, COLORS::SECONDARY);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, COLORS::SECONDARY);

    std::thread t([]() {
        GLOBALS::scanner.config.init();
    });
    t.detach();

    bool show_stats = false;
    bool show_demo_window = false;
    bool show_console = false;
    bool drawerOpened = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (ImGui::IsKeyPressed(GLFW_KEY_F12) && !io.WantCaptureKeyboard) {
            show_stats = !show_stats;
        }
        if (ImGui::IsKeyPressed(GLFW_KEY_GRAVE_ACCENT) && !io.WantCaptureKeyboard) {
            show_demo_window = !show_demo_window;
        }
        if (ImGui::IsKeyPressed(GLFW_KEY_F1) && !io.WantCaptureKeyboard) {
            show_console = !show_console;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        ConsoleWindow(show_console);
        StatsInfo(show_stats);
        LoadingScreen();
        Overlay();
        ApiKeyPrompt();
        SideBar();
        TopBar();
        MainBody();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        // This makes absolutely no sense but for some reason
        // opening the drawer for 1 frame fixes a memory leak
        if (!drawerOpened) {
            GLOBALS::scanner.showDrawer = false;
            drawerOpened = true;
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
