//
// Created by mecha on 31.08.2022.
//

#include <graphics/core/TextureMixer.h>

namespace engine::graphics {

    TextureMixer::TextureMixer(const FrameBufferFormat& frameBufferFormat) {
        // init shader
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "texture_mixer",
                        "primitive_quad.glsl",
                        "f_mix.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        // init frame buffer
        frameBuffer = createRef<FrameBuffer>();
        frameBuffer->updateFormat(frameBufferFormat);
    }

    u32 TextureMixer::render(const vector<u32> &textures) {
        frameBuffer->bind();
        shaderProgram.start();

        for (u32 i = 0; i < textures.size(); i++) {
            TextureBuffer::activate(i);
            TextureBuffer::bind(textures[i], TextureBuffer::getTypeId(TextureType::TEXTURE_2D));
            IntUniform sampler = IntUniform("textures", (int)i);
            BoolUniform enabled = BoolUniform("enabled", true);
            shaderProgram.setUniformArrayElement(i, sampler);
            shaderProgram.setUniformArrayElement(i, enabled);
        }
        drawV(DrawType::TRIANGLE_STRIP, 4);

        shaderProgram.stop();
        ColorAttachment mixedTexture;
        frameBuffer->getColorAttachment(0, mixedTexture);
        return mixedTexture.id;
    }

    void TextureMixer::release() {
        frameBuffer->destroy();
        shaderProgram.release();
    }

}