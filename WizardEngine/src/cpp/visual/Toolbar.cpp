#include <visual/Toolbar.h>
#include <core/Application.h>
#include <imgui.h>

namespace engine::visual {

    Toolbar::Toolbar() {
        TextureData playTD = TextureFile::read("assets/textures/PlayButton.png");
        playIcon = TextureBuffer::upload(playTD);
        io::TextureFile::free(playTD);

        TextureData stopTD = TextureFile::read("assets/textures/StopButton.png");
        stopIcon = TextureBuffer::upload(stopTD);
        io::TextureFile::free(stopTD);

        TextureData pauseTD = TextureFile::read("assets/textures/PauseButton.png");
        pauseIcon = TextureBuffer::upload(pauseTD);
        io::TextureFile::free(pauseTD);

        TextureData simulateTD = TextureFile::read("assets/textures/SimulateButton.png");
        simulateIcon = TextureBuffer::upload(simulateTD);
        io::TextureFile::free(simulateTD);

        TextureData stepTD = TextureFile::read("assets/textures/StepButton.png");
        stepIcon = TextureBuffer::upload(stepTD);
        io::TextureFile::free(stepTD);
    }

    Toolbar::~Toolbar() {
        TextureBuffer::destroy(playIcon);
        TextureBuffer::destroy(stopIcon);
        TextureBuffer::destroy(pauseIcon);
        TextureBuffer::destroy(simulateIcon);
        TextureBuffer::destroy(stepIcon);
    }

    void Toolbar::draw() {
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

        auto& activeScene = Application::get().activeScene;
        if (activeScene) {
            sceneState = activeScene->getState();
            isPaused = activeScene->isPaused;
        } else {
            sceneState = SceneState::EDIT;
            isPaused = false;
        }

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
            ImGui::SameLine();
            {
                if (ImGui::ImageButton((ImTextureID) pauseIcon,
                                       ImVec2(size, size),
                                       ImVec2(0, 0), ImVec2(1, 1),
                                       0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                                       tintColor) && enabled)
                {
                    isPaused = !isPaused;
                    if (activeScene) {
                        activeScene->isPaused = isPaused;
                    }
                }
            }

            // Step button
            if (isPaused)
            {
                ImGui::SameLine();
                {
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

        auto& scene = Application::get().activeScene;
        if (scene) {
            scene->setState(sceneState);
            scene->isPaused = isPaused;
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void Toolbar::onScenePlay() {
        sceneState = SceneState::PLAY;
        isPaused = false;
    }

    void Toolbar::onSceneStop() {
        sceneState = SceneState::EDIT;
        isPaused = true;
    }

    void Toolbar::onSceneStep() {

    }

    void Toolbar::onSceneSimulate() {
        sceneState = SceneState::SIMULATE;
        isPaused = false;
    }
}