//
// Created by mecha on 25.03.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/core/geometry/Cube.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>
#include <graphics/core/Renderer.h>

namespace engine::graphics {

    enum class SkyboxType {
        CUBE = 0,
        SPHERE = 1
    };

    struct SkyboxVertex {
        math::vec3f position = { 0.5, 0.5 , 0.5 };
    };

    serialize_component(Skybox) {
        SkyboxType type = SkyboxType::CUBE;
        VertexDataComponent<SkyboxVertex> geometry;
        CubeMapTextureComponent textures;

        Skybox() = default;

        Skybox(SkyboxType type) : type(type) {
            init();
        }

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;

    private:
        void init() {
            switch (type) {
                case SkyboxType::CUBE:
                    initCube();
                    break;
                case SkyboxType::SPHERE:
                    break;
            }
        }

        void initCube() {
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

            geometry.vertexData = { vertices, 0, 36 };
            geometry.drawType = DrawType::TRIANGLE;
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
            add<Skybox>(SkyboxType::CUBE);
            add<CubeMapTextureComponent>(cubeMapTextures);
        }
    };

    class ENGINE_API SkyboxRenderer : public VRenderer<SkyboxVertex> {

    public:
        SkyboxRenderer() : VRenderer<SkyboxVertex>() {}

    public:
        void init();

    protected:
        void uploadUniforms(const ecs::Entity& entity) override;
    };

}
