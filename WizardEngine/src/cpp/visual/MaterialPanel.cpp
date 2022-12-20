#include <visual/MaterialPanel.h>
#include <visual/Widgets.h>

namespace engine::visual {

    MaterialTextures MaterialPanel::materialTextures;
    Ref<FileDialog> MaterialPanel::fileDialog;

    void MaterialPanel::create(void* nativeWindow) {
        MaterialPanel::fileDialog = createRef<FileDialog>(nativeWindow);
    }

    void MaterialPanel::draw(Material& material) {
        static bool open = true;
        if (!ImGui::Begin(material.title, &open)) {
            ImGui::End();
            return;
        }
        drawMaterial(material);
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
                drawMaterial(*material);
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
                drawMaterial(*material);
            }
        }

        ImGui::End();
    }

    void MaterialPanel::drawMaterial(Material &material) {
        // base color
        ColorPicker::draw(material.color);
        Slider::draw(material.gamma, { -5, 5 });
        Slider::draw(material.brightness, { -1, 1 });
        drawTextureMap(0, &materialTextures.albedoPath, "Albedo", material.albedoSlot.textureId, material.enableAlbedoMap);
        // normals
        drawTextureMap(1, &materialTextures.normalPath, "Normals", material.normalSlot.textureId, material.enableNormalMap);
        // parallax/depth
        drawTextureMap(4, &materialTextures.parallaxPath, "Parallax", material.depthSlot.textureId, material.enableParallaxMap);
        Slider::draw(material.heightScale, { 0, 10 });
        Slider::draw(material.minLayers, { 0, 10 });
        Slider::draw(material.maxLayers, { 10, 20 });
        // metallic
        drawTextureMap(5, &materialTextures.metallicPath, "Metallic", material.metallicSlot.textureId, material.enableMetallicMap);
        Slider::draw(material.metallic, { -5, 5 });
        // roughness
        drawTextureMap(6, &materialTextures.roughnessPath, "Roughness", material.roughnessSlot.textureId, material.enableRoughnessMap);
        Slider::draw(material.roughness, { -5, 5 });
        // AO
        drawTextureMap(7, &materialTextures.aoPath, "Ambient Occlusion", material.aoSlot.textureId, material.enableAOMap);
        Slider::draw(material.ao, { -5, 5 });
    }

    void MaterialPanel::draw(const Batch3d &entity) {
        Material* material = entity.get<Material>();
        if (material) {
            drawMaterial(*material);
        }
    }

    void MaterialPanel::drawTextureMap(int index, std::string* filepath,
                                       const char* title,
                                       u32 &textureId, BoolUniform& textureEnabled, io::Spectrum spectrum) {

        Line::draw(title);
        Checkbox::draw(title, textureEnabled.value);
        ImGui::PushID(index);
        if (ImGui::ImageButton(
                reinterpret_cast<ImTextureID>(textureId),
                { 128, 128 },
                { 1, 1 },
                { 0, 0 })
        ) {
            const char* filter = "PNG image (*.png)\0*.png\0"
                                 "JPG image (*.jpg)\0*.jpg\0"
                                 "TGA image (*.tga)\0*.tga\0";
            *filepath = fileDialog->getImportPath(filter);
            textureId = TextureBuffer::load(filepath->c_str(), spectrum);
        }
        ImGui::PopID();
    }

}