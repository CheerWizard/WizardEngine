//
// Created by mecha on 25.03.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/core/geometry/Cube.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>
#include <graphics/core/Renderer.h>
#include <graphics/camera/Camera.h>

namespace engine::graphics {

    enum class SkyboxType {
        CUBE = 0,
        SPHERE = 1
    };

    struct SkyboxVertex {
        math::vec3f position = { 0.5, 0.5 , 0.5 };
    };

    component(Skybox) {
        serializable()
        SkyboxType type = SkyboxType::CUBE;
        VertexDataComponent<SkyboxVertex> geometry;
        CubeMapTextureComponent textures;
        Transform3dComponent transform = {
                {0,   0,   0},
                {0,   0,   0},
                {100, 100, 100}
        };

        Skybox() = default;

        Skybox(SkyboxType type) : type(type) {
            init();
        }

        Skybox(SkyboxType type, const CubeMapTextureComponent& textures) : type(type), textures(textures) {
            init();
        }

        void rotate(const vec3f& rotation) {
            transform.modelMatrix.rotation += rotation;
            transform.modelMatrix.apply();
        }

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
        SkyboxCube(
                const std::string& tag,
                ecs::EntityContainer* container,
                const CubeMapTextureComponent& cubeMapTextures
        ) : ecs::Entity(tag, container) {
            init(cubeMapTextures);
        }

        ~SkyboxCube() = default;

    private:
        void init(const CubeMapTextureComponent& cubeMapTextures) {
            add<Skybox>(SkyboxType::CUBE, cubeMapTextures);
        }
    };

    class ENGINE_API SkyboxRenderer {

    public:
        SkyboxRenderer() = default;

    public:
        void init();
        void release();
        void upload(Skybox* skybox);
        void render(const ecs::Entity& entity, Camera3D& camera);

    private:
        BaseShaderProgram shaderProgram;
        VRenderModel renderModel;
    };

}
