//
// Created by mecha on 25.03.2022.
//

#include <graphics/core/texture/Texture.h>
#include <platform/graphics/TextureBuffer.h>

namespace engine::graphics {

    void TextureComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "TextureComponent";
        yaml::serialize(out, "textureId", textureId);
        yaml::serialize(out, "typeId", typeId);
        yaml::serialize(out, "sampler", sampler);
        out << YAML::EndMap;
    }

    void TextureComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["TextureComponent"];
        if (root) {
            yaml::deserialize(root, "textureId", textureId);
            yaml::deserialize(root, "typeId", typeId);
            yaml::deserialize(root, "sampler", sampler);
        }
    }

    void CubeMapTextureComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "CubeMapTextureComponent";
        yaml::serialize(out, "textureId", textureId);
        yaml::serialize(out, "typeId", typeId);
        yaml::serialize(out, "sampler", sampler);
        out << YAML::EndMap;
    }

    void CubeMapTextureComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["CubeMapTextureComponent"];
        if (root) {
            yaml::deserialize(root, "textureId", textureId);
            yaml::deserialize(root, "typeId", typeId);
            yaml::deserialize(root, "sampler", sampler);
        }
    }

    ShaderScript textureScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, ecs::Registry &registry) {
            registry.each<TextureComponent>([&shader](TextureComponent* texture) {
                shader.setUniformArrayElement(texture->sampler.value, texture->sampler);
                texture->sampler.isUpdated = false;
                TextureBuffer::bind(texture->textureId, texture->typeId);
                TextureBuffer::activate(texture->sampler.value);
            });
        };

        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto texture = entity.get<TextureComponent>();
            if (texture) {
                shader.setUniformArrayElement(texture->sampler.value, texture->sampler);
                TextureBuffer::bind(texture->textureId, texture->typeId);
                TextureBuffer::activate(texture->sampler.value);
            }
        };

        return script;
    }

    ShaderScript textureUboScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, ecs::Registry &registry) {
            registry.each<TextureComponent>([&shader](TextureComponent* texture) {
                shader.updateUniformBuffer(texture->sampler, 0);
                TextureBuffer::bind(texture->textureId, texture->typeId);
                TextureBuffer::activate(texture->sampler.value);
            });
        };

        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto texture = entity.get<TextureComponent>();
            if (texture) {
                shader.updateUniformBuffer(texture->sampler, 0);
                TextureBuffer::bind(texture->textureId, texture->typeId);
                TextureBuffer::activate(texture->sampler.value);
            }
        };

        return script;
    }

    ShaderScript cubeMapTextureScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, ecs::Registry &registry) {
            // do nothing
        };

        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto texture = entity.get<CubeMapTextureComponent>();
            if (texture) {
                shader.setUniform(texture->sampler);
                TextureBuffer::activate(texture->sampler.value);
                TextureBuffer::bind(texture->textureId, texture->typeId);
            }
        };

        return script;
    }

}