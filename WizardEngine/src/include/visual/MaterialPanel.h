#pragma once

#include <visual/Visual.h>
#include <graphics/materials/Material.h>
#include <graphics/GraphicsObject.h>

#include <platform/tools/FileDialog.h>

namespace engine::visual {

    using namespace engine::graphics;
    using namespace engine::tools;

    struct MaterialTextures {
        std::string albedoPath = "";
        std::string normalPath = "";
        std::string diffusePath = "";
        std::string specularPath = "";
        std::string parallaxPath = "";
        std::string metallicPath = "";
        std::string roughnessPath = "";
        std::string aoPath = "";
    };

    class ENGINE_API MaterialPanel final {
    public:
        static void create(void* nativeWindow);
        static void draw(Material& material);
        static void draw(const vector<ecs::Entity>& entities);
        static void draw(const vector<Batch3d>& entities);
        static void draw(const Batch3d& entity);

    public:
        static void drawMaterial(Material& material);

    private:
        static void drawTextureMap(int id, std::string* filepath,
                                   const char* title,
                                   u32& textureId, BoolUniform& textureEnabled, io::Spectrum spectrum = io::Spectrum::NONE);

    private:
        static MaterialTextures materialTextures;
        static Ref<FileDialog> fileDialog;
    };

}
