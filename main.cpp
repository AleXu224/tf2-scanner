#include "components/MainBody.hpp"
#include "components/Overlay.hpp"
#include "components/SideBar.hpp"
#include "components/StatsInfo.hpp"
#include "components/TopBar.hpp"
#include "fonts/MaterialIcons.cpp"
#include "fonts/Roboto.cpp"
#include "globals.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include "resources/bpscannerIcon.hpp"
#include "GLFW/glfw3.h"
#include "imgui_internal.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int, char**) {
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(800, 600, "TF2 Scanner", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLFWimage icon;
    icon.pixels = (unsigned char*)ProgramIcon_data;
    icon.height = 256;
    icon.width = 256;
    glfwSetWindowIcon(window, 1, &icon);

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
        GLOBALS::scanner.config.checkForUpdates();
    });
    t.detach();

    bool show_demo_window = false;

    // Overlays added at the bottom so they will always be under the other overlays
    // These will also not be removed when the overlay is closed
    Overlay::addOverlay(new StatusIndicator());
    Overlay::addOverlay(new StatsInfo());
    Overlay::addOverlay(new ConsoleWindow());
    Overlay::addOverlay(new SideBar());

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (ImGui::IsKeyPressed(GLFW_KEY_F12) && !io.WantCaptureKeyboard) {
            GLOBALS::scanner.showStats = !GLOBALS::scanner.showStats;
        }
        if (ImGui::IsKeyPressed(GLFW_KEY_GRAVE_ACCENT) && !io.WantCaptureKeyboard) {
            show_demo_window = !show_demo_window;
        }
        if (ImGui::IsKeyPressed(GLFW_KEY_F1) && !io.WantCaptureKeyboard) {
            GLOBALS::scanner.showConsole = !GLOBALS::scanner.showConsole;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        MainBody();
        TopBar();
        Overlay::draw();
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // sort imgui windows by submission order
        // this allows us to actually have a z-order
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        std::stable_sort(ctx->Windows.begin(), ctx->Windows.end(), [](const ImGuiWindow* lhs, const ImGuiWindow* rhs) {
            return lhs->BeginOrderWithinContext < rhs->BeginOrderWithinContext;
        });

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
