//
// Created by mecha on 25.03.2022.
//

#include <graphics/core/texture/Texture.h>
#include <graphics/core/sources/TextureSource.h>

namespace engine::graphics {

    ShaderScript textureScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry &registry) {
            auto textures = registry.view<TextureComponent>();
            for (auto [entity, texture] : textures.each()) {
                shader.setUniformArrayElement(texture.sampler.value, texture.sampler);
                texture.sampler.isUpdated = false;
                ACTIVATE_TEXTURE(texture);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto texture = entity.getPtr<TextureComponent>();
            if (texture) {
                shader.setUniformArrayElement(texture->sampler.value, texture->sampler);
                ACTIVATE_TEXTURE(*texture);
            }
        };
        return script;
    }

    ShaderScript textureUboScript() {
        auto script = ShaderScript();
        script.updateRegistry = [](const BaseShader& shader, entt::registry &registry) {
            auto textures = registry.view<TextureComponent>();
            for (auto [entity, texture] : textures.each()) {
                shader.updateUniformBuffer(texture.sampler, 0);
                ACTIVATE_TEXTURE(texture);
            }
        };
        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto texture = entity.getPtr<TextureComponent>();
            if (texture) {
                shader.updateUniformBuffer(texture->sampler, 0);
                ACTIVATE_TEXTURE(*texture);
            }
        };
        return script;
    }

    ShaderScript cubeMapTextureScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, entt::registry &registry) {
            // do nothing
        };

        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto texture = entity.getPtr<CubeMapTextureComponent>();
            if (texture) {
                shader.setUniform(texture->sampler);
                ACTIVATE_TEXTURE(*texture);
            }
        };

        return script;
    }

}