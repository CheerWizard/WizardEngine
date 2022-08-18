//
// Created by mecha on 25.03.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/core/geometry/Cube.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>

namespace engine::graphics {

    struct SkyboxVertex {
        math::vec3f position = { 0.5, 0.5 , 0.5 };
    };

    struct SkyCube : VertexDataComponent<SkyboxVertex> {

        SkyCube() {
            init();
        }

        void init() {
            auto vertices = new SkyboxVertex[36] {
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f,  1.0f, -1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{-1.0f, -1.0f, -1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f,  1.0f,  1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{-1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f, -1.0f,  1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{1.0f,  1.0f, -1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{-1.0f,  1.0f,  1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f, -1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{1.0f, -1.0f,  1.0f}},
            };

            this->vertexData = { vertices, 0, 36 };
            this->drawType = DrawType::TRIANGLE;
        }
    };

    class SkyboxCube : public ecs::Entity {

    public:
        SkyboxCube(const std::string& tag, ecs::EntityContainer* container, const CubeMapTextureComponent& cubeMapTextures)
        : ecs::Entity(tag, container) {
            init(cubeMapTextures);
        }

        ~SkyboxCube() = default;

    private:
        void init(const CubeMapTextureComponent& cubeMapTextures) {
            add<Transform3dComponent>(Transform3dComponent{
                    {0,   0,   0},
                    {0,   0,   0},
                    {100, 100, 100}
            });
            add<SkyCube>(SkyCube());
            add<CubeMapTextureComponent>(cubeMapTextures);
        }
    };

}
