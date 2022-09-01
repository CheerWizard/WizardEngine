//
// Created by mecha on 17.08.2022.
//

#include <graphics/skybox/Skybox.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

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