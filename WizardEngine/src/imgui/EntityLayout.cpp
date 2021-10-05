//
// Created by mecha on 03.10.2021.
//

#include "EntityLayout.h"

#include "imgui.h"

namespace engine {

    void EntityLayout::onUpdate(Time deltaTime) {
        ENGINE_INFO("onUpdate()");
        ImGui::Begin(_props.name);
        // todo define list of entities and buttons to manipulate with them.
        ImGui::End();
    }

    void EntityLayout::destroy() {
        removeCallback();
    }

}