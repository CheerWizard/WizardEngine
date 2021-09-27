//
// Created by mecha on 05.09.2021.
//

#include "DemoLayout.h"

#include "imgui.h"

namespace engine {

    void DemoLayout::onUpdate(Time deltaTime) {
        Layer::onUpdate(deltaTime);

        if (_show) {
            ImGui::ShowDemoWindow(&_show);
        }
    }

    void DemoLayout::onKeyPressed(KeyCode keyCode) {
        Layer::onKeyPressed(keyCode);
        if (showKeyPressed == keyCode) {
            _show = !_show;
        }
    }

}