//
// Created by mecha on 02.12.2022.
//

#include <visual/LightPanel.h>
#include <visual/Widgets.h>

namespace engine::visual {

    void LightsPanel::draw(const vector<PhongLight> &lights) {
        static bool open = true;
        if (!ImGui::Begin("LightsPanel", &open)) {
            ImGui::End();
            return;
        }

        for (const auto& light : lights) {
            PhongLightComponent* phongLight = light.get<PhongLightComponent>();
            if (phongLight) {
                ColorPicker::draw(phongLight->color);
                Controller::draw(phongLight->position, 0, 200);
            }
        }

        ImGui::End();
    }

}