//
// Created by mecha on 01.10.2021.
//

#include "ImageLayout.h"

#include <imgui.h>

namespace engine {

    void ImageLayout::onUpdate(Time deltaTime) {
        ImGui::Begin(_props.name);
        ImGui::Image(
        (void*) _props.textureId,
        ImVec2 {
            (float) _props.width,
            (float) _props.height
            }
        );
        ImGui::End();
    }

}