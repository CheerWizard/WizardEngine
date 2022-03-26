//
// Created by mecha on 25.03.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/core/geometry/Cube.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>
#include <glm/glm.hpp>

namespace engine {

    struct SkyboxVertex {
        glm::vec3 position = { 0.5, 0.5 , 0.5 };
    };

    class SkyboxCube : public Entity {

    public:
        SkyboxCube(const std::string& tag, EntityContainer* container, const CubeMapTextureComponent& cubeMapTextures)
        : Entity(tag, container) {
            init(cubeMapTextures);
        }

        ~SkyboxCube() = default;

    private:
        void init(const CubeMapTextureComponent& cubeMapTextures) {
            add<Transform3dComponent>(transform3d(
                    { 0, 0, 0 },
                    { 3.15, 0, 0 },
                    { 100, 100, 100 }
            ));
            add<VertexDataComponent<SkyboxVertex>>(Cube<SkyboxVertex>());
            add<CubeMapTextureComponent>(cubeMapTextures);
        }
    };

}
