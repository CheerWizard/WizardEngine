//
// Created by mecha on 30.08.2022.
//

#include <graphics/core/post_processing/PostEffect.h>

namespace engine::graphics {

    u32 PostEffectRenderer::render(u32 textureId) {
        if (!postEffect->enabled) {
            ENGINE_WARN("PostEffectRenderer: {0} is disabled!", postEffect->name);
            return textureId;
        }

        frameBuffer->bind();
        shaderProgram.start();

        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureType::TEXTURE_2D);
        updatePostEffect();
        drawV(DrawType::TRIANGLE_STRIP, 4);

        shaderProgram.stop();
        ColorAttachment texture;
        frameBuffer->getColorAttachment(0, texture);
        return texture.id;
    }

    void PostEffectRenderer::updatePostEffect() {
        auto& effect = *postEffect;
        // upload primitives
        for (auto primitive : effect.floats) {
            shaderProgram.setUniformStructField(effect.name, primitive);
        }
        for (auto& primitive : effect.doubles) {
            shaderProgram.setUniformStructField(effect.name, primitive);
        }
        for (auto primitive : effect.ints) {
            shaderProgram.setUniformStructField(effect.name, primitive);
        }
        for (auto primitive : effect.flags) {
            shaderProgram.setUniformStructField(effect.name, primitive);
        }
        // upload vectors
        for (auto vec : effect.vec4) {
            shaderProgram.setUniformStructField(effect.name, vec);
        }
        // upload textures
        for (auto texture : effect.textures) {
            if (texture.isValid()) {
                shaderProgram.setUniformStructField(effect.name, texture.sampler);
                TextureBuffer::activate(texture.sampler.value);
                TextureBuffer::bind(texture.textureId, texture.typeId);
            }
        }
    }

    void PostEffectRenderer::release() {
        frameBuffer->destroy();
        shaderProgram.release();
    }
}