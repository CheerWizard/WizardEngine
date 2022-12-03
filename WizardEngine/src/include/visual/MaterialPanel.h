#pragma once

#include <visual/Visual.h>
#include <graphics/materials/Material.h>
#include <graphics/GraphicsObject.h>

namespace engine::visual {

    using namespace engine::graphics;

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
        static void draw(Material& material);
        static void draw(const vector<ecs::Entity>& entities);
        static void draw(const vector<Batch3d>& entities);
        static void draw(const Batch3d& entity);

    private:
        static void drawInternal(Material& material);
        static void drawTextureMap(std::string* filepath,
                                   const char* field_id,
                                   const char* title,
                                   u32& textureId, BoolUniform& textureEnabled);

    private:
        static MaterialTextures materialTextures;
    };

}
