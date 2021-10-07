//
// Created by mecha on 03.10.2021.
//

#include "EntityLayout.h"

#include "imgui.h"

namespace engine {

    void EntityLayout::onUpdate(Time deltaTime) {
        ENGINE_INFO("onUpdate()");
        ImGui::Begin(_props.name, nullptr, ImGuiWindowFlags_NoCollapse);

        if (ImGui::Button("Create Entity")) {
            if (_callback != nullptr) {
                _callback->createEntity("Entity");
            }
        }

        static const char* items[] {
            "Entity1",
            "Entity2",
            "Entity3"
        };
        static int selectedItem = 0;
        ImGui::ListBox("", &selectedItem, items, IM_ARRAYSIZE(items));

        ImGui::End();
    }

    void EntityLayout::destroy() {
        removeCallback();
    }

}