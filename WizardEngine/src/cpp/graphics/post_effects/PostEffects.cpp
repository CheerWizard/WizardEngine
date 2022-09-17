//
// Created by mecha on 30.08.2022.
//

#include <graphics/post_effects/PostEffects.h>

namespace engine::graphics {

    HdrEffectRenderer::HdrEffectRenderer(const FrameBufferFormat &frameBufferFormat)
    : PostEffectRenderer() {
        // init shader
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "hdr",
                        "primitive_quad.glsl",
                        "/post_processing/f_hdr.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        // init frame buffer
        frameBuffer = createRef<FrameBuffer>();
        frameBuffer->updateFormat(frameBufferFormat);
        // init post effect
        postEffect = createRef<HdrEffect>();
    }

    BlurEffectRenderer::BlurEffectRenderer(const FrameBufferFormat &frameBufferFormat)
    : PostEffectRenderer() {
        // init shader
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "blur",
                        "primitive_quad.glsl",
                        "/post_processing/f_blur.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        // init frame buffer
        frameBuffer = createRef<FrameBuffer>();
        frameBuffer->updateFormat(frameBufferFormat);
        // init post effect
        postEffect = createRef<BlurEffect>();
    }

    SharpenEffectRenderer::SharpenEffectRenderer(const FrameBufferFormat &frameBufferFormat)
    : PostEffectRenderer() {
        // init shader
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "sharpen",
                        "primitive_quad.glsl",
                        "/post_processing/f_sharpen.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        // init frame buffer
        frameBuffer = createRef<FrameBuffer>();
        frameBuffer->updateFormat(frameBufferFormat);
        // init post effect
        postEffect = createRef<SharpenEffect>();
    }

    EdgeDetectionEffectRenderer::EdgeDetectionEffectRenderer(const FrameBufferFormat &frameBufferFormat)
    : PostEffectRenderer() {
        // init shader
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "edge_detection",
                        "primitive_quad.glsl",
                        "/post_processing/f_edge_detection.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        // init frame buffer
        frameBuffer = createRef<FrameBuffer>();
        frameBuffer->updateFormat(frameBufferFormat);
        // init post effect
        postEffect = createRef<EdgeDetectionEffect>();
    }

    GaussianBlurEffectRenderer::GaussianBlurEffectRenderer(const FrameBufferFormat &frameBufferFormat)
    : PostEffectRenderer() {
        // init shader
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "gaussian_blur",
                        "primitive_quad.glsl",
                        "/post_processing/f_gaussian_blur.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        // init frame buffer
        frameBuffer = createRef<FrameBuffer>();
        frameBuffer->updateFormat(frameBufferFormat);
        horizontalFrameBuffer = createRef<FrameBuffer>();
        horizontalFrameBuffer->updateFormat(frameBufferFormat);
        // init post effect
        postEffect = createRef<GaussianBlurEffect>();
    }

    u32 GaussianBlurEffectRenderer::render(u32 textureId) {
        ENGINE_INFO("GaussianBlurEffectRenderer::render()");

        if (!postEffect->enabled) {
            ENGINE_WARN("GaussianBlurEffectRenderer: {0} is disabled!", postEffect->name);
            return textureId;
        }

        shaderProgram.start();

        horizontalFrameBuffer->bind();
        TextureBuffer::activate(0);
        TextureBuffer::bind(textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D));
        BoolUniform horizontalUniform = { "horizontal", true };
        shaderProgram.setUniform(horizontalUniform);
        drawV(DrawType::TRIANGLE_STRIP, 4);
        horizontalUniform.value = !horizontalUniform.value;

        ColorAttachment horizontalTexture;
        horizontalFrameBuffer->getColorAttachment(0, horizontalTexture);
        textureId = horizontalTexture.id;

        u32 amount = getGaussianBlurEffect()->amount;
        for (u32 i = 1 ; i < amount ; i++) {
            Ref<FrameBuffer> fbo = horizontalUniform.value ? horizontalFrameBuffer : frameBuffer;
            fbo->bind();
            TextureBuffer::activate(0);
            TextureBuffer::bind(textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D));
            shaderProgram.setUniform(horizontalUniform);
            drawV(DrawType::TRIANGLE_STRIP, 4);
            horizontalUniform.value = !horizontalUniform.value;

            ColorAttachment texture;
            fbo->getColorAttachment(0, texture);
            textureId = texture.id;
        }

        shaderProgram.stop();
        return textureId;
    }
}