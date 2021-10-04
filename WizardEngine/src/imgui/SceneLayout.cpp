//
// Created by mecha on 01.10.2021.
//

#include "SceneLayout.h"

#include <imgui.h>

namespace engine {

    void SceneLayout::onUpdate(Time deltaTime) {
        Layer::onUpdate(deltaTime);
        ImGui::Begin(props.name);
        auto colorTexture = scene->getTextureId();
        ImGui::Image((void*) colorTexture, ImVec2 {(float) props.width,(float) props.height });
        ImGui::End();
    }

    void SceneLayout::destroy() {
        scene = nullptr;
    }

}