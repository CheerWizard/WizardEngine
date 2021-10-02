//
// Created by mecha on 01.10.2021.
//

#include "SceneLayout.h"

#include <imgui.h>

namespace engine {

    void SceneLayout::onUpdate(Time deltaTime) {
        Layer::onUpdate(deltaTime);
        ImGui::Begin(_props.name);
        auto colorTexture = _scene->getColorTexture();
        ImGui::Image((void*) colorTexture, ImVec2 {(float) _props.width,(float) _props.height });
        ImGui::End();
    }

}