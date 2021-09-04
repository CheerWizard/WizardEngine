//
// Created by mecha on 31.08.2021.
//

#include "ImguiLayer.h"

namespace engine {

    ImGuiLayer::ImGuiLayer() : Layer("ImguiLayer") {

    }

    void ImGuiLayer::onAttach() {
        Layer::onAttach();

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

    void ImGuiLayer::onDetach() {
        Layer::onDetach();
    }

    void ImGuiLayer::onUpdate(Time deltaTime) {
        Layer::onUpdate(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();

        float time = (float) glfwGetTime();
        io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
        _time = time;

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    ImGuiLayer::~ImGuiLayer() {

    }

    void ImGuiLayer::onEvent(Event &e) {
        Layer::onEvent(e);
    }

}
