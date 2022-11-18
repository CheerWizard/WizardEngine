//
// Created by mecha on 17.08.2022.
//

#include <graphics/skybox/Skybox.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    void Skybox::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "Skybox";

        yaml::serialize(out, "type", static_cast<u32>(type));
        textures.serialize(out);

        out << YAML::EndMap;
    }

    void Skybox::deserialize(const YAML::Node &parent) {
        auto root = parent["Skybox"];
        if (root) {
            type = static_cast<SkyboxType>(root["type"].as<u32>());
            textures.deserialize(parent);
            init();
        }
    }

    void SkyboxRenderer::init() {
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "skybox",
                        "v_cubemap.glsl",
                        "f_cubemap.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        shaderProgram.bindVertexFormat();
    }

    void SkyboxRenderer::uploadUniforms(const ecs::Entity& entity) {
        ENGINE_INFO("SkyboxRenderer::uploadUniforms()");
        auto camera = entity.get<Camera3dComponent>();
        auto cubemap = entity.get<CubeMapTextureComponent>();

        if (camera) {
            shaderProgram.getVShader().updateUniformBuffer(camera->viewProjection, 0);
        }

        if (cubemap) {
            shaderProgram.setUniform(cubemap->sampler);
            TextureBuffer::activate(cubemap->sampler.value);
            TextureBuffer::bind(cubemap->textureId, cubemap->typeId);
        }
    }

}