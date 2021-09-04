//
// Created by mecha on 31.08.2021.
//

#include "ImguiLayer.h"
#include "../core/Application.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

#include "../core/KeyCodes.h"
#include "../core/MouseCodes.h"

#define IO ImGui::GetIO()

namespace engine {

    ImGuiLayer::ImGuiLayer() : Layer("ImguiLayer") {
    }

    void ImGuiLayer::onCreate() {
        Layer::onCreate();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        // setup IO controller for ImGui
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Keyboard mapping. Dear ImGui will use those indices to peek into the io.KeysDown[] array.
        io.KeyMap[ImGuiKey_Tab] = Tab;
        io.KeyMap[ImGuiKey_LeftArrow] = Left;
        io.KeyMap[ImGuiKey_RightArrow] = Right;
        io.KeyMap[ImGuiKey_UpArrow] = Up;
        io.KeyMap[ImGuiKey_DownArrow] = Down;
        io.KeyMap[ImGuiKey_PageUp] = PageUp;
        io.KeyMap[ImGuiKey_PageDown] = PageDown;
        io.KeyMap[ImGuiKey_Home] = Home;
        io.KeyMap[ImGuiKey_End] = End;
        io.KeyMap[ImGuiKey_Insert] = Insert;
        io.KeyMap[ImGuiKey_Delete] = Delete;
        io.KeyMap[ImGuiKey_Backspace] = Backspace;
        io.KeyMap[ImGuiKey_Space] = Space;
        io.KeyMap[ImGuiKey_Enter] = Enter;
        io.KeyMap[ImGuiKey_Escape] = Escape;
        io.KeyMap[ImGuiKey_KeyPadEnter] = KPEnter;
        io.KeyMap[ImGuiKey_A] = A;
        io.KeyMap[ImGuiKey_C] = C;
        io.KeyMap[ImGuiKey_V] = V;
        io.KeyMap[ImGuiKey_X] = X;
        io.KeyMap[ImGuiKey_Y] = Y;
        io.KeyMap[ImGuiKey_Z] = Z;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onUpdate(Time deltaTime) {
        Layer::onUpdate(deltaTime);

        Application& app = Application::getInstance();

        auto windowWidth = app.getWindow().getWidth();
        auto windowHeight = app.getWindow().getHeight();

        IO.DisplaySize = ImVec2(windowWidth, windowHeight);

        auto time = (float) glfwGetTime();
        IO.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
        _time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::onDestroy() {
        Layer::onDestroy();
    }

    void ImGuiLayer::onImGuiRender() {
        Layer::onImGuiRender();
    }

    void ImGuiLayer::onWindowClosed() {
        Layer::onWindowClosed();
    }

    void ImGuiLayer::onWindowResized(unsigned int width, unsigned int height) {
        Layer::onWindowResized(width, height);
        IO.DisplaySize = ImVec2(width, height);
        IO.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    }

    void ImGuiLayer::onKeyPressed(KeyCode keyCode) {
        Layer::onKeyPressed(keyCode);

        IO.KeysDown[keyCode] = true;

        IO.KeyCtrl = IO.KeysDown[LeftControl] || IO.KeysDown[RightControl];
        IO.KeyShift = IO.KeysDown[LeftShift] || IO.KeysDown[RightShift];
        IO.KeyAlt = IO.KeysDown[LeftAlt] || IO.KeysDown[RightAlt];
        IO.KeySuper = IO.KeysDown[LeftSuper] || IO.KeysDown[RightSuper];
    }

    void ImGuiLayer::onKeyHold(KeyCode keyCode) {
        Layer::onKeyHold(keyCode);
    }

    void ImGuiLayer::onKeyReleased(KeyCode keyCode) {
        Layer::onKeyReleased(keyCode);
        IO.KeysDown[keyCode] = false;
    }

    void ImGuiLayer::onMousePressed(MouseCode mouseCode) {
        Layer::onMousePressed(mouseCode);
        IO.MouseDown[mouseCode] = true;
    }

    void ImGuiLayer::onMouseRelease(MouseCode mouseCode) {
        Layer::onMouseRelease(mouseCode);
        IO.MouseDown[mouseCode] = false;
    }

    void ImGuiLayer::onMouseScrolled(double xOffset, double yOffset) {
        Layer::onMouseScrolled(xOffset, yOffset);
        IO.MouseWheelH += xOffset;
        IO.MouseWheel += yOffset;
    }

    void ImGuiLayer::onCursorMoved(double xPos, double yPos) {
        Layer::onCursorMoved(xPos, yPos);
        IO.MousePos = ImVec2(xPos, yPos);
    }

    void ImGuiLayer::onKeyTyped(KeyCode keyCode) {
        Layer::onKeyTyped(keyCode);
        if (keyCode > 0 && keyCode < 0x10000) {
            IO.AddInputCharacter(keyCode);
        }
    }

}
