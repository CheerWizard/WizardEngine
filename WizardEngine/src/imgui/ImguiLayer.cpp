//
// Created by mecha on 31.08.2021.
//

#include "ImguiLayer.h"
#include "../core/Application.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#define IO ImGui::GetIO()
#define STYLE ImGui::GetStyle()
#define COLORS STYLE.Colors

namespace engine {

    ImGuiLayer::ImGuiLayer() : Layer("ImguiLayer") {
    }

    void ImGuiLayer::onCreate() {
        Layer::onCreate();
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

//        io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
//        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            STYLE.WindowRounding = 0.0f;
            STYLE.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        setDarkTheme();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(GLFW_WINDOW, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onUpdate(Time deltaTime) {
        Layer::onUpdate(deltaTime);
        onBeginFrame();
        _layoutStack.onUpdate(deltaTime);
        onEndFrame();
    }

    void ImGuiLayer::onBeginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::onEndFrame() {
        IO.DisplaySize = ImVec2((float) WINDOW->getWidth(), (float) WINDOW->getHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::onDestroy() {
        Layer::onDestroy();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::setDarkTheme() {
        COLORS[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        COLORS[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        COLORS[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        COLORS[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        COLORS[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        COLORS[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        COLORS[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        COLORS[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        COLORS[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        COLORS[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        COLORS[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        COLORS[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        COLORS[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        COLORS[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        COLORS[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        COLORS[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        COLORS[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        COLORS[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

    void ImGuiLayer::pushLayout(Layout *imGuiLayout) {
        _layoutStack.pushLayer(imGuiLayout);
    }

    void ImGuiLayer::pushOverLayout(Layout *imGuiLayout) {
        _layoutStack.pushOverlay(imGuiLayout);
    }

    void ImGuiLayer::popLayout(Layout *imGuiLayout) {
        _layoutStack.popLayer(imGuiLayout);
    }

    void ImGuiLayer::popOverLayout(Layout *imGuiLayout) {
        _layoutStack.popOverlay(imGuiLayout);
    }

    void ImGuiLayer::onWindowClosed() {
        Layer::onWindowClosed();
        _layoutStack.onWindowClosed();
    }

    void ImGuiLayer::onWindowResized(unsigned int width, unsigned int height) {
        Layer::onWindowResized(width, height);
        _layoutStack.onWindowResized(width, height);
    }

    void ImGuiLayer::onKeyPressed(KeyCode keyCode) {
        Layer::onKeyPressed(keyCode);
        _layoutStack.onKeyPressed(keyCode);
    }

    void ImGuiLayer::onKeyHold(KeyCode keyCode) {
        Layer::onKeyHold(keyCode);
        _layoutStack.onKeyHold(keyCode);
    }

    void ImGuiLayer::onKeyReleased(KeyCode keyCode) {
        Layer::onKeyReleased(keyCode);
        _layoutStack.onKeyReleased(keyCode);
    }

    void ImGuiLayer::onKeyTyped(KeyCode keyCode) {
        Layer::onKeyTyped(keyCode);
        _layoutStack.onKeyTyped(keyCode);
    }

    void ImGuiLayer::onMousePressed(MouseCode mouseCode) {
        Layer::onMousePressed(mouseCode);
        _layoutStack.onMousePressed(mouseCode);
    }

    void ImGuiLayer::onMouseRelease(MouseCode mouseCode) {
        Layer::onMouseRelease(mouseCode);
        _layoutStack.onMouseRelease(mouseCode);
    }

    void ImGuiLayer::onMouseScrolled(double xOffset, double yOffset) {
        Layer::onMouseScrolled(xOffset, yOffset);
        _layoutStack.onMouseScrolled(xOffset, yOffset);
    }

    void ImGuiLayer::onCursorMoved(double xPos, double yPos) {
        Layer::onCursorMoved(xPos, yPos);
        _layoutStack.onCursorMoved(xPos, yPos);
    }

}
