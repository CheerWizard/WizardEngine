#include <visual/MaterialPanel.h>
#include <visual/Widgets.h>

namespace engine::visual {

    MaterialTextures MaterialPanel::materialTextures;

    void MaterialPanel::draw(Material& material) {
        static bool open = true;
        if (!ImGui::Begin(material.title, &open)) {
            ImGui::End();
            return;
        }
        drawInternal(material);
        ImGui::End();
    }

    void MaterialPanel::draw(const vector<ecs::Entity>& entities) {
        static bool open = true;
        if (!ImGui::Begin("Materials", &open)) {
            ImGui::End();
            return;
        }

        for (auto entity : entities) {
            Material* material = entity.get<Material>();
            if (material) {
                drawInternal(*material);
            }
        }

        ImGui::End();
    }

    void MaterialPanel::draw(const vector<Batch3d> &entities) {
        static bool open = true;
        if (!ImGui::Begin("Materials", &open)) {
            ImGui::End();
            return;
        }

        for (auto entity : entities) {
            Material* material = entity.get<Material>();
            if (material) {
                drawInternal(*material);
            }
        }

        ImGui::End();
    }

    void MaterialPanel::drawInternal(Material &material) {
        // base color
        ColorPicker::draw(material.color);
        Slider::draw(material.gamma, { -5, 5 });
        Slider::draw(material.brightness, { -1, 1 });
        Checkbox::draw(material.enableBlinn);
        Slider::draw(material.ambient, { -5, 5 });
        drawTextureMap(&materialTextures.albedoPath, "albedo_path_field", "Albedo Map", material.albedoSlot.textureId, material.enableAlbedoMap);
        // normals
        drawTextureMap(&materialTextures.normalPath, "normal_path_field", "Normal Map", material.normalSlot.textureId, material.enableNormalMap);
        // diffuse
        drawTextureMap(&materialTextures.diffusePath, "diffuse_path_field", "Diffuse Map", material.diffuseSlot.textureId, material.enableDiffuseMap);
        Slider::draw(material.diffuse, { -5, 5 });
        // specular
        drawTextureMap(&materialTextures.specularPath, "specular_path_field", "Specular Map", material.specularSlot.textureId, material.enableSpecularMap);
        Slider::draw(material.specular, { -5, 5 });
        Slider::draw(material.shiny, { -100, 100 });
        // parallax/depth
        drawTextureMap(&materialTextures.parallaxPath, "parallax_path_field", "Parallax Map", material.depthSlot.textureId, material.enableParallaxMap);
        Slider::draw(material.heightScale, { 0, 10 });
        Slider::draw(material.minLayers, { 0, 10 });
        Slider::draw(material.maxLayers, { 10, 20 });
        // metallic
        drawTextureMap(&materialTextures.metallicPath, "metallic_path_field", "Metallic Map", material.metallicSlot.textureId, material.enableMetallicMap);
        Slider::draw(material.metallic, { -5, 5 });
        // roughness
        drawTextureMap(&materialTextures.roughnessPath, "roughness_path_field", "Roughness Map", material.roughnessSlot.textureId, material.enableRoughnessMap);
        Slider::draw(material.roughness, { -5, 5 });
        // AO
        drawTextureMap(&materialTextures.aoPath, "ao_path_field", "AO Map", material.aoSlot.textureId, material.enableAOMap);
        Slider::draw(material.ao, { -5, 5 });
    }

    void MaterialPanel::draw(const Batch3d &entity) {
        Material* material = entity.get<Material>();
        if (material) {
            draw(*material);
        }
    }

    void MaterialPanel::drawTextureMap(std::string* filepath,
                                       const char* field_id,
                                       const char* title,
                                       u32 &textureId, BoolUniform& textureEnabled) {
        if (ImGui::ImageButton(
                reinterpret_cast<ImTextureID>(textureId),
                {32, 32 },
                { 1, 1 },
                { 0, 0 })
        ) {
            textureId = TextureBuffer::load(filepath->c_str());
        }
        ImGui::SameLine();
        Text::field(field_id, title, filepath);
        ImGui::SameLine();
        Checkbox::draw(textureEnabled);
    }

}