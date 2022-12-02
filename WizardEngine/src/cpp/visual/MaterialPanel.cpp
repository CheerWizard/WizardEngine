#include <visual/MaterialPanel.h>
#include <visual/Widgets.h>

namespace engine::visual {

    void MaterialPanel::draw(Material& material) {
        static bool open = true;
        if (!ImGui::Begin(material.title, &open)) {
            ImGui::End();
            return;
        }

        ColorPicker::draw(material.color);
        Slider::draw(material.gamma, { 0, 5 });
        Slider::draw(material.brightness, { 0, 1 });
        Checkbox::draw(material.enableBlinn);
        Slider::draw(material.ambient, { 0, 1 });
        Checkbox::draw(material.enableAlbedoMap);
        Slider::draw(material.diffuse, { 0, 1 });
        Checkbox::draw(material.enableNormalMap);
        Checkbox::draw(material.enableDiffuseMap);
        Slider::draw(material.specular, { 0, 1 });
        Slider::draw(material.shiny, { 0, 32 });
        Checkbox::draw(material.enableSpecularMap);
        Slider::draw(material.heightScale, { 0, 1 });
        Slider::draw(material.minLayers, { 0, 8 });
        Slider::draw(material.maxLayers, { 8, 32 });
        Checkbox::draw(material.enableParallaxMap);
        Slider::draw(material.metallic, { 0, 1 });
        Checkbox::draw(material.enableMetallicMap);
        Slider::draw(material.roughness, { 0, 1 });
        Checkbox::draw(material.enableRoughnessMap);
        Slider::draw(material.ao, { 0, 1 });
        Checkbox::draw(material.enableAOMap);

        ImGui::End();
    }

}