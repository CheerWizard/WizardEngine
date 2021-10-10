//
// Created by mecha on 31.08.2021.
//

#include "ImguiLayer.h"
#include "../platform/Platform.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

#define IO ImGui::GetIO()
#define STYLE ImGui::GetStyle()
#define COLORS STYLE.Colors

namespace engine {

    bool ImGuiLayer::isFullScreen = true;
    bool ImGuiLayer::isDockSpaceOpened = true;
    int ImGuiLayer::windowFlags = 0;
    int ImGuiLayer::dockSpaceFlags = 0;

    void ImGuiLayer::create() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        boldFont = IO.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Bold.ttf", 16.0f);
        regularFont = IO.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 16.0f);
        resizableFont = IO.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 20.0f);
        // set default font!
        IO.FontDefault = regularFont;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            STYLE.WindowRounding = 0.0f;
            COLORS[ImGuiCol_WindowBg].w = 1.0f;
        }

        setDarkTheme();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) app->getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onUpdate(Time dt) {
        onBeginFrame();
        beginDockSpace();
        onRender(dt);
        endDockSpace();
        onEndFrame();
    }

    void ImGuiLayer::onBeginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::onEndFrame() {
        IO.DisplaySize = ImVec2((float) props.width, (float) props.height);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::destroy() {
        boldFont = nullptr;
        regularFont = nullptr;
        resizableFont = nullptr;
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        app = nullptr;
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

    void ImGuiLayer::setDockSpaceOption(const bool &isFullscreen) {
        isFullScreen = isFullscreen;
    }

    void ImGuiLayer::openDockSpace() {
        isDockSpaceOpened = true;
    }

    void ImGuiLayer::hideDockSpace() {
        isDockSpaceOpened = false;
    }

    void ImGuiLayer::beginDockSpace() {
        if (!isDockSpaceOpened) return;
        static bool isDockSpaceOpened = isDockSpaceOpened;

        setDockSpace();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("WizardDockSpace", &isDockSpaceOpened, windowFlags);
        ImGui::PopStyleVar();

        if (isFullScreen) {
            ImGui::PopStyleVar(2);
        }

        if (IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("WizardDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockSpaceFlags);
        }
    }

    void ImGuiLayer::setDockSpace() {
        dockSpaceFlags = ImGuiDockNodeFlags_None;
        windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (isFullScreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }
    }

    void ImGuiLayer::endDockSpace() {
        if (!isDockSpaceOpened) return;
        ImGui::End();
    }

    void ImGuiLayer::toggleDockSpace() {
        isDockSpaceOpened = !isDockSpaceOpened;
    }

    void ImGuiLayer::onPrepare() {
    }

    void ImGuiLayer::onKeyPressed(KeyCode keyCode) {
        Layer::onKeyPressed(keyCode);
        IO.KeysDown[keyCode] = true;
    }

    void ImGuiLayer::onKeyHold(KeyCode keyCode) {
        Layer::onKeyHold(keyCode);
        IO.KeysDown[keyCode] = true;
    }

    void ImGuiLayer::onKeyReleased(KeyCode keyCode) {
        Layer::onKeyReleased(keyCode);
        IO.KeysDown[keyCode] = false;
    }

    void ImGuiLayer::onKeyTyped(KeyCode keyCode) {
        Layer::onKeyTyped(keyCode);
        IO.AddInputCharacter(keyCode);
    }

    void ImGuiLayer::onMouseScrolled(double xOffset, double yOffset) {
        Layer::onMouseScrolled(xOffset, yOffset);
        IO.MouseWheelH += xOffset;
        IO.MouseWheel += yOffset;
    }

    void ImGuiLayer::onMousePressed(MouseCode mouseCode) {
        Layer::onMousePressed(mouseCode);
        IO.MouseDown[mouseCode] = true;
    }

    void ImGuiLayer::onMouseRelease(MouseCode mouseCode) {
        Layer::onMouseRelease(mouseCode);
        IO.MouseDown[mouseCode] = false;
    }

    void ImGuiLayer::onCursorMoved(double xPos, double yPos) {
        Layer::onCursorMoved(xPos, yPos);
        IO.MousePos = {(float) xPos, (float) yPos};
    }

}
