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

    void LightsPanel::draw(const vector<Light> &lights) {
        static bool open = true;
        if (!ImGui::Begin("LightsPanel", &open)) {
            ImGui::End();
            return;
        }

        for (const auto& light : lights) {
            LightComponent* lightComponent = light.get<LightComponent>();
            if (lightComponent) {
                ColorPicker::draw(lightComponent->color);
                // X axis
                Text::label("X");
                ImGui::SameLine();
                Slider::draw("##X", lightComponent->position.value.v[0], { -100, 100 });
                // Y axis
                Text::label("Y");
                ImGui::SameLine();
                Slider::draw("##Y", lightComponent->position.value.v[1], { -100, 100 });
                // Z axis
                Text::label("Z");
                ImGui::SameLine();
                Slider::draw("##Z", lightComponent->position.value.v[2], { -100, 100 });
            }
        }

        ImGui::End();
    }

}