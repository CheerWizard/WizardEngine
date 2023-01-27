//
// Created by mecha on 01.09.2022.
//

#include <visual/Visual.h>
#include <visual/FontAwesome4.h>
#include <visual/ProjectsPanel.h>

#include <profiler/Profiler.h>

#include <platform/graphics/RenderCommands.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include <imnodes.h>

#include <core/Application.h>

#define IO ImGui::GetIO()
#define STYLE ImGui::GetStyle()
#define COLORS STYLE.Colors

namespace engine::visual {

    vector<ImFont*> Visual::fonts;
    bool Visual::fullScreen;
    bool Visual::openDockspace;
    bool Visual::blockEvents = true;
    int Visual::windowFlags;
    int Visual::dockspaceFlags;
    Ref<FileDialog> Visual::s_FileDialog;

    void Visual::init(void* nativeWindow) {
        PROFILE_FUNCTION()

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImNodes::CreateContext();
        IO.IniFilename = "Visual.ini";

        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            STYLE.WindowRounding = 0.0f;
            COLORS[ImGuiCol_WindowBg].w = 1.0f;
        }

        IO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        IO.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        IO.KeyMap[ImGuiKey_Tab] = KeyCode::Tab;
        IO.KeyMap[ImGuiKey_LeftArrow] = KeyCode::Left;
        IO.KeyMap[ImGuiKey_RightArrow] = KeyCode::Right;
        IO.KeyMap[ImGuiKey_UpArrow] = KeyCode::Up;
        IO.KeyMap[ImGuiKey_DownArrow] = KeyCode::Down;
        IO.KeyMap[ImGuiKey_PageUp] = KeyCode::PageUp;
        IO.KeyMap[ImGuiKey_PageDown] = KeyCode::PageDown;
        IO.KeyMap[ImGuiKey_Home] = KeyCode::Home;
        IO.KeyMap[ImGuiKey_End] = KeyCode::End;
        IO.KeyMap[ImGuiKey_Insert] = KeyCode::Insert;
        IO.KeyMap[ImGuiKey_Delete] = KeyCode::Delete;
        IO.KeyMap[ImGuiKey_Backspace] = KeyCode::Backspace;
        IO.KeyMap[ImGuiKey_Space] = KeyCode::Space;
        IO.KeyMap[ImGuiKey_Enter] = KeyCode::Enter;
        IO.KeyMap[ImGuiKey_Escape] = KeyCode::Escape;
        IO.KeyMap[ImGuiKey_A] = KeyCode::A;
        IO.KeyMap[ImGuiKey_C] = KeyCode::C;
        IO.KeyMap[ImGuiKey_V] = KeyCode::V;
        IO.KeyMap[ImGuiKey_X] = KeyCode::X;
        IO.KeyMap[ImGuiKey_Y] = KeyCode::Y;
        IO.KeyMap[ImGuiKey_Z] = KeyCode::Z;

        IO.MouseDrawCursor = true;

        setTheme();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) nativeWindow, true);
        ImGui_ImplOpenGL3_Init("#version 400 core");

        s_FileDialog = createRef<FileDialog>(nativeWindow);
    }

    u32 Visual::addFont(const char *filepath, f32 fontSize) {
        fonts.emplace_back(IO.Fonts->AddFontFromFileTTF(filepath, fontSize));
        return fonts.size() - 1;
    }

    void Visual::setDefaultFont(u32 fontIndex) {
        if (fontIndex >= fonts.size()) {
            ENGINE_ERR("Visual::setDefaultFont: invalid condition fontIndex > fonts.size()");
            return;
        }
        IO.FontDefault = fonts.at(fontIndex);
    }

    void Visual::setDefaultFont(const char *filepath, f32 fontSize) {
        setDefaultFont(addFont(filepath, fontSize));
    }

    void Visual::setTheme() {
        IO.Fonts->Clear();
        setDefaultFont("assets/fonts/opensans/OpenSans-Regular.ttf", 16);
        // load icon font
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;

        IO.Fonts->AddFontFromFileTTF(
                "assets/fonts/font_awesome_4/fontawesome-webfont.ttf",
                18,
                &icons_config, icons_ranges);
        IO.Fonts->Build();

        ImGuiStyle* style = &ImGui::GetStyle();

        style->WindowPadding            = ImVec2(15, 15);
        style->WindowRounding           = 5.0f;
        style->FramePadding             = ImVec2(5, 5);
        style->FrameRounding            = 4.0f;
        style->ItemSpacing              = ImVec2(12, 8);
        style->ItemInnerSpacing         = ImVec2(8, 6);
        style->IndentSpacing            = 25.0f;
        style->ScrollbarSize            = 15.0f;
        style->ScrollbarRounding        = 9.0f;
        style->GrabMinSize              = 5.0f;
        style->GrabRounding             = 3.0f;

        style->Colors[ImGuiCol_Text]                  = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.39f, 0.38f, 0.77f);
        style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.92f, 0.91f, 0.88f, 0.70f);
        style->Colors[ImGuiCol_ChildBg]               = ImVec4(1.00f, 0.98f, 0.95f, 0.58f);
        style->Colors[ImGuiCol_PopupBg]               = ImVec4(0.92f, 0.91f, 0.88f, 0.92f);
        style->Colors[ImGuiCol_Border]                = ImVec4(0.84f, 0.83f, 0.80f, 0.65f);
        style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        style->Colors[ImGuiCol_FrameBg]               = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 1.00f, 1.00f, 0.78f);
        style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_TitleBg]               = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
        style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
        style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(1.00f, 0.98f, 0.95f, 0.47f);
        style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.00f, 0.00f, 0.00f, 0.21f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.25, 1.00f, 1.00f, 0.78f);
        style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.25f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.25f, 1.00f, 1.00f, 0.80f);
        style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
        style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.25f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_Button]                = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
        style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.25f, 1.00f, 1.00f, 0.86f);
        style->Colors[ImGuiCol_ButtonActive]          = ImVec4(0.25f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_Header]                = ImVec4(0.25f, 1.00f, 1.00f, 0.76f);
        style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 1.00f, 1.00f, 0.86f);
        style->Colors[ImGuiCol_HeaderActive]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
        style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.25f, 1.00f, 1.00f, 0.78f);
        style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.25f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_PlotLines]             = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.25f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.25f, 1.00f, 1.00f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.25f, 1.00f, 1.00f, 0.43f);
        style->Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

        ImGui::StyleColorsDark(style);
    }

    void Visual::begin() {
        auto& app = Application::get();
        IO.DisplaySize = { (float)app.getWindowWidth(), (float)app.getWindowHeight() };
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Visual::end() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void Visual::release() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImNodes::DestroyContext();
        ImGui::DestroyContext();
    }

    void Visual::onKeyPressed(event::KeyCode keyCode) {
        IO.KeysDown[keyCode] = true;
        IO.KeyCtrl = IO.KeysDown[KeyCode::LeftControl] || IO.KeysDown[KeyCode::RightControl];
        IO.KeyShift = IO.KeysDown[KeyCode::LeftShift] || IO.KeysDown[KeyCode::RightShift];
        IO.KeyAlt = IO.KeysDown[KeyCode::LeftAlt] || IO.KeysDown[KeyCode::RightAlt];
        IO.KeySuper = IO.KeysDown[KeyCode::LeftSuper] || IO.KeysDown[KeyCode::RightSuper];
    }

    void Visual::onKeyHold(event::KeyCode keyCode) {
    }

    void Visual::onKeyReleased(event::KeyCode keyCode) {
        IO.KeysDown[keyCode] = false;
    }

    void Visual::onKeyTyped(event::KeyCode keyCode) {
        if (keyCode > 0 && keyCode < 0x10000)
            IO.AddInputCharacter(keyCode);
    }

    void Visual::onMouseScrolled(double xOffset, double yOffset) {
        IO.MouseWheelH += (float)xOffset;
        IO.MouseWheel += (float)yOffset;
    }

    void Visual::onMousePressed(event::MouseCode mouseCode) {
        IO.MouseDown[mouseCode] = true;
    }

    void Visual::onMouseRelease(event::MouseCode mouseCode) {
        IO.MouseDown[mouseCode] = false;
    }

    void Visual::onCursorMoved(double xPos, double yPos) {
        IO.MousePos = { (float)xPos, (float)yPos };
    }

    void Visual::onWindowClosed() {
    }

    void Visual::onWindowResized(u32 width, u32 height) {
        IO.DisplaySize = { (float)width, (float)height };
        IO.DisplayFramebufferScale = { 1, 1 };
        setViewPort(0, 0, (int)width, (int)height);
    }

    void Visual::sameLine() {
        ImGui::SameLine();
    }

    void Visual::separator() {
        ImGui::Separator();
    }

    void Visual::drawDockspace() {
        static bool open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Wizard Dockspace", &open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        // Menu Bar
        if (ImGui::BeginMenuBar()) {
            // Scene Menu
            if (ImGui::BeginMenu("Scene")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    auto& app = Application::get();
                    std::stringstream ss;
                    ss << "New Scene " << LocalAssetManager::getSceneSize();
                    app.newScene(ss.str());
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    auto importPath = s_FileDialog->getImportPath("YAML Scene (*.yaml)\0*.yaml\0");
                    LocalAssetManager::read(importPath.c_str());
                }

                if (ImGui::MenuItem("Save All", "Ctrl+S")) {
                    LocalAssetManager::writeAll("assets/scenes");
                }

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                    auto exportPath = s_FileDialog->getExportPath("YAML Scene (*.yaml)\0*.yaml\0");
                    LocalAssetManager::write(Application::get().activeScene, exportPath.c_str());
                }

                ImGui::EndMenu();
            }
            // Projects Menu
            if (ImGui::BeginMenu("Projects")) {
                if (ImGui::MenuItem("Panel", "Ctrl+P")) {
                    ProjectsPanel::get().enableDraw = true;
                }

                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    ProjectsPanel::get().enableDraw = true;
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    ProjectsPanel::get().enableDraw = true;
                }

                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    ProjectsPanel::get().enableDraw = true;
                }

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                    ProjectsPanel::get().enableDraw = true;
                }

                ImGui::EndMenu();
            }
            // Menu Bar
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    bool Visual::blocksKeyboard() {
        return IO.WantCaptureKeyboard && blockEvents;
    }

    bool Visual::blocksMouse() {
        return IO.WantCaptureMouse && blockEvents;
    }

    bool Visual::blocksTextInput() {
        return IO.WantTextInput && blockEvents;
    }

    bool Visual::blocksMousePos() {
        return IO.WantSetMousePos && blockEvents;
    }

    void Visual::onUpdate(Time dt) {
        IO.DeltaTime = dt.seconds();
    }

    void Panel::begin(const char* title, const vec2f& size) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0 , 0});
        ImGui::SetNextWindowSize({ size.x(), size.y() });
        static bool open = true;
        if (!ImGui::Begin(title, &open)) {
            end();
            return;
        }
    }

    void Panel::end() {
        ImGui::End();
        ImGui::PopStyleVar();
    }

    bool Panel::isFocused(ImGuiFocusedFlags flags) {
        return ImGui::IsWindowFocused(flags);
    }

    void Gizmo::enable(bool enabled) {
        ImGuizmo::Enable(enabled);
    }

    void draw(
            Camera3D& camera,
            Transform3dComponent& transform,
            const ImGuizmo::OPERATION& operation,
            const ImGuizmo::MODE& mode,
            const vec2f& windowPosition,
            const vec2f& windowSize
    ) {
        ENGINE_INFO("Gizmo::draw()");
//        ImGuizmo::SetOrthographic(false);
//        ImGuizmo::SetDrawlist();
//        ImGuiIO& io = ImGui::GetIO();
//        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
//        ImGuizmo::Manipulate(
//                toFloatPtr(camera.getView()),
//                toFloatPtr(camera.getPerspective()),
//                operation,
//                mode,
//                toFloatPtr(transform.modelMatrix)
//        );
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(
                math::values(transform.modelMatrix.value),
                matrixTranslation, matrixRotation, matrixScale
        );
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(
                matrixTranslation, matrixRotation, matrixScale,
                math::values(transform.modelMatrix.value)
        );
        static bool useSnap(false);
        if (ImGui::IsKeyPressed(83)) useSnap = !useSnap;
        ImGui::SameLine();
        vec3f snap;
        switch (operation) {
            case ImGuizmo::TRANSLATE:
                ImGui::InputFloat3("Snap", math::values(snap));
                break;
            case ImGuizmo::ROTATE:
                ImGui::InputFloat("Angle Snap", math::values(snap));
                break;
            case ImGuizmo::SCALE:
                ImGui::InputFloat("Scale Snap", math::values(snap));
                break;
        }
        ImGuizmo::SetRect(windowPosition.x(), windowPosition.y(), windowSize.x(), windowSize.y());
        camera.getView().apply();
        camera.getPerspective().apply();
        ImGuizmo::Manipulate(math::values(camera.getView().value), math::values(camera.getPerspective().value),
                             operation, mode, math::values(transform.modelMatrix.value),
                             NULL, useSnap ? math::values(snap) : NULL);
    }

    void Gizmo::drawTranslate(Camera3D& camera, Transform3dComponent& transform,
                              const vec2f& windowPosition, const vec2f& windowSize) {
        draw(camera, transform, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, windowPosition, windowSize);
    }

    void Gizmo::drawRotate(Camera3D &camera, Transform3dComponent &transform,
                           const vec2f& windowPosition, const vec2f& windowSize) {
        draw(camera, transform, ImGuizmo::ROTATE, ImGuizmo::LOCAL, windowPosition, windowSize);
    }

    void Gizmo::drawScale(Camera3D &camera, Transform3dComponent &transform,
                          const vec2f& windowPosition, const vec2f& windowSize) {
        draw(camera, transform, ImGuizmo::SCALE, ImGuizmo::LOCAL, windowPosition, windowSize);
    }
}