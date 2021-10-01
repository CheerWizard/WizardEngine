//
// Created by mecha on 01.10.2021.
//

#include "ScenePanel.h"

#include <imgui.h>

namespace engine {

    void ScenePanel::onUpdate(Time deltaTime) {
        Layer::onUpdate(deltaTime);
        ImGui::Begin("Scene Panel");
        auto colorTexture = _scene->getColorTexture();
        ImGui::Image((void*) colorTexture, ImVec2 {(float) _width,(float) _height });
        ImGui::End();
    }

}