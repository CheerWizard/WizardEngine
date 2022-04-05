//
// Created by mecha on 31.08.2021.
//

#include <imgui/ImguiLayer.h>
#include <imgui/theme.h>
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
        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        boldFont = IO.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Bold.ttf", 16.0f);
        regularFont = IO.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 16.0f);
        resizableFont = IO.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 20.0f);
        // set default font!
        IO.FontDefault = regularFont;

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            STYLE.WindowRounding = 0.0f;
            COLORS[ImGuiCol_WindowBg].w = 1.0f;
        }
        setDarkTheme();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) app->getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init(GRAPHICS_CORE_VERSION);
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
        ImGui::StyleColorsLight(&STYLE);
        auto& colors = STYLE.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.8f, 0.8f, 0.8f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.7f, 0.7f, 0.7f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.8f, 0.8f, 0.8f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.7f, 0.7f, 0.7f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.8f, 0.8f, 0.8f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.7f, 0.7f, 0.7f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.62f, 0.62f, 0.62f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.72f, 0.72f, 0.72f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.8f, 0.8f, 0.8f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.09f, 0.09f, 0.09f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.95f);

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

        // Check Mark
        colors[ImGuiCol_CheckMark] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);

        // Slider
        colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.34f, 0.34f, 0.34f, 1.0f);
    }

    void ImGuiLayer::setDarkThemeV2() {
        auto& style = ImGui::GetStyle();
        auto& colors = ImGui::GetStyle().Colors;

        //========================================================
        /// Colours

        // Headers
        colors[ImGuiCol_Header]				= ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);
        colors[ImGuiCol_HeaderHovered]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);
        colors[ImGuiCol_HeaderActive]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);

        // Buttons
        colors[ImGuiCol_Button]				= ImColor(56, 56, 56, 200);
        colors[ImGuiCol_ButtonHovered]		= ImColor(70, 70, 70, 255);
        colors[ImGuiCol_ButtonActive]		= ImColor(56, 56, 56, 150);

        // Frame BG
        colors[ImGuiCol_FrameBg]			= ImGui::ColorConvertU32ToFloat4(Colours::Theme::propertyField);
        colors[ImGuiCol_FrameBgHovered]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::propertyField);
        colors[ImGuiCol_FrameBgActive]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::propertyField);

        // Tabs
        colors[ImGuiCol_Tab]				= ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
        colors[ImGuiCol_TabHovered]			= ImColor(255, 225, 135, 30);
        colors[ImGuiCol_TabActive]			= ImColor(255, 225, 135, 60);
        colors[ImGuiCol_TabUnfocused]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
        colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabHovered];

        // Title
        colors[ImGuiCol_TitleBg]			= ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
        colors[ImGuiCol_TitleBgActive]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
        colors[ImGuiCol_TitleBgCollapsed]	= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Resize Grip
        colors[ImGuiCol_ResizeGrip]			= ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered]	= ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]	= ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg]		= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]		= ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

        // Check Mark
        colors[ImGuiCol_CheckMark]			= ImColor(200, 200, 200, 255);

        // Slider
        colors[ImGuiCol_SliderGrab]			= ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
        colors[ImGuiCol_SliderGrabActive]	= ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

        // Text
        colors[ImGuiCol_Text]				= ImGui::ColorConvertU32ToFloat4(Colours::Theme::text);

        // Checkbox
        colors[ImGuiCol_CheckMark]			= ImGui::ColorConvertU32ToFloat4(Colours::Theme::text);

        // Separator
        colors[ImGuiCol_Separator]			= ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundDark);
        colors[ImGuiCol_SeparatorActive]	= ImGui::ColorConvertU32ToFloat4(Colours::Theme::highlight);
        colors[ImGuiCol_SeparatorHovered]	= ImColor(39, 185, 242, 150);

        // Window Background
        colors[ImGuiCol_WindowBg]			= ImGui::ColorConvertU32ToFloat4(Colours::Theme::titlebar);
        colors[ImGuiCol_ChildBg]			= ImGui::ColorConvertU32ToFloat4(Colours::Theme::background);
        colors[ImGuiCol_PopupBg]			= ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundPopup);
        colors[ImGuiCol_Border]				= ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundDark);

        // Tables
        colors[ImGuiCol_TableHeaderBg]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);
        colors[ImGuiCol_TableBorderLight]	= ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundDark);

        // Menubar
        colors[ImGuiCol_MenuBarBg]			= ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };

        //========================================================
        /// Style
        style.FrameRounding = 2.5f;
        style.FrameBorderSize = 1.0f;
        style.IndentSpacing = 11.0f;
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
