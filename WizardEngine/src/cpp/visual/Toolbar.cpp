#include <visual/Toolbar.h>
#include <core/Application.h>
#include <imgui.h>

namespace engine::visual {

    Toolbar::Toolbar() {
        playIcon = TextureBuffer::load("assets/textures/PlayButton.png");
        stopIcon = TextureBuffer::load("assets/textures/StopButton.png");
        pauseIcon = TextureBuffer::load("assets/textures/PauseButton.png");
        simulateIcon = TextureBuffer::load("assets/textures/SimulateButton.png");
        stepIcon = TextureBuffer::load("assets/textures/StepButton.png");
    }

    Toolbar::~Toolbar() {
        TextureBuffer::destroy(playIcon);
        TextureBuffer::destroy(stopIcon);
        TextureBuffer::destroy(pauseIcon);
        TextureBuffer::destroy(simulateIcon);
        TextureBuffer::destroy(stepIcon);
    }

    void Toolbar::draw() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                }

                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                }

                if (ImGui::MenuItem("Exit"))
                    Application::get().onWindowClosed();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec4 tintColor = ImVec4(1, 1, 1, 1);
        if (!enabled)
            tintColor.w = 0.5f;

        float size = ImGui::GetWindowHeight() - 4.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

        Ref<Scene> activeScene = Application::get().activeScene;
        auto sceneState = activeScene->getState();

        bool hasPlayButton = sceneState == SceneState::EDIT || sceneState == SceneState::PLAY;
        bool hasSimulateButton = sceneState == SceneState::EDIT || sceneState == SceneState::SIMULATE;
        bool hasPauseButton = sceneState != SceneState::EDIT;

        if (hasPlayButton) {
            u32 icon = (sceneState == SceneState::EDIT || sceneState == SceneState::SIMULATE) ? playIcon : stopIcon;
            if (ImGui::ImageButton((ImTextureID) icon,
                                   ImVec2(size, size),
                                   ImVec2(0, 0), ImVec2(1, 1),
                                   0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                                   tintColor) && enabled)
            {
                if (sceneState == SceneState::EDIT || sceneState == SceneState::SIMULATE)
                    onScenePlay();
                else if (sceneState == SceneState::PLAY)
                    onSceneStop();
            }
        }

        if (hasSimulateButton) {
            if (hasPlayButton)
                ImGui::SameLine();

            u32 icon = (sceneState == SceneState::EDIT || sceneState == SceneState::PLAY) ? simulateIcon : stopIcon;
            if (ImGui::ImageButton(
                    (ImTextureID) icon,
                    ImVec2(size, size),
                    ImVec2(0, 0),
                    ImVec2(1, 1),
                    0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && enabled)
            {
                if (sceneState == SceneState::EDIT || sceneState == SceneState::PLAY)
                    onSceneSimulate();
                else if (sceneState == SceneState::SIMULATE)
                    onSceneStop();
            }
        }

        if (hasPauseButton) {
            bool isPaused = activeScene->isPaused;
            ImGui::SameLine();
            {
                if (ImGui::ImageButton((ImTextureID) pauseIcon,
                                       ImVec2(size, size),
                                       ImVec2(0, 0), ImVec2(1, 1),
                                       0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                                       tintColor) && enabled)
                {
                    activeScene->isPaused = !isPaused;
                }
            }

            // Step button
            if (isPaused)
            {
                ImGui::SameLine();
                {
                    bool isPaused = activeScene->isPaused;
                    if (ImGui::ImageButton((ImTextureID) stepIcon,
                                           ImVec2(size, size),
                                           ImVec2(0, 0), ImVec2(1, 1),
                                           0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                                           tintColor) && enabled)
                    {
                        onSceneStep();
                    }
                }
            }
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void Toolbar::onScenePlay() {

    }

    void Toolbar::onScenePause() {

    }

    void Toolbar::onSceneStop() {

    }

    void Toolbar::onSceneStep() {

    }

    void Toolbar::onSceneSimulate() {

    }
}