#pragma once

#include <ecs/Entity.h>
#include <graphics/core/geometry/Cube.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>
#include <graphics/core/Renderer.h>
#include <graphics/camera/Camera.h>
#include <platform/graphics/FrameBuffer.h>

namespace engine::graphics {

    enum class HdrEnvType {
        CUBE = 0,
        SPHERE = 1
    };

    struct HdrEnvVertex {
        math::vec3f position = { 0.5, 0.5 , 0.5 };
    };

    component(HdrEnv) {
        serializable()
        HdrEnvType type = HdrEnvType::CUBE;
        VertexDataComponent<HdrEnvVertex> geometry;
        TextureComponent texture;
        Transform3dComponent transform = {
                {0,   0,   0},
                {0,   0,   0},
                {100, 100, 100}
        };

        HdrEnv() = default;

        HdrEnv(HdrEnvType type) : type(type) {
            init();
        }

        HdrEnv(HdrEnvType type, const TextureComponent& texture) : type(type), texture(texture) {
            init();
        }

        void rotate(const vec3f& rotation) {
            transform.modelMatrix.rotation += rotation;
            transform.modelMatrix.apply();
        }

    private:
        void init() {
            switch (type) {
                case HdrEnvType::CUBE:
                    initCube();
                    break;
                case HdrEnvType::SPHERE:
                    break;
            }
        }

        void initCube() {
            auto vertices = new HdrEnvVertex[36] {
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

    class HdrEnvCube : public ecs::Entity {

    public:
        HdrEnvCube(
                const std::string& tag,
                ecs::EntityContainer* container,
                const TextureComponent& texture
        ) : ecs::Entity(tag, container) {
            init(texture);
        }

        ~HdrEnvCube() = default;

    private:
        void init(const TextureComponent& textureComponent) {
            add<HdrEnv>(HdrEnvType::CUBE, textureComponent);
        }
    };

    class ENGINE_API HdrEnvRenderer final {

    public:
        HdrEnvRenderer() = default;

    public:
        void init();
        void release();
        void upload(HdrEnv* hdrEnv);
        void render(const ecs::Entity& entity, Camera3D& camera);

    private:
        BaseShaderProgram generateCubemapShader;
        BaseShaderProgram cubemapShader;
        VRenderModel renderModel;
        Ref<FrameBuffer> frameBuffer;
        u32 cubemap = 0;
    };

}