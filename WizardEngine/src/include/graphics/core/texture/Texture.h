//
// Created by mecha on 25.03.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>

#define CHANNEL_RED 0
#define CHANNEL_RGB 3
#define CHANNEL_RGBA 4

namespace engine::graphics {

    using namespace core;

    // texture format for color buffer
    enum class ColorFormat {
        NONE = 0,
        RGBA8,
        RED_INTEGER
    };

    // texture format for depth and stencil buffers
    enum class DepthStencilFormat {
        NONE = 0,
        DEPTH16,
        DEPTH24PAD8,
        DEPTH24STENCIL8,
        DEPTH32
    };

    enum class TextureParamName {
        MIN_FILTER, MAG_FILTER,
        WRAP_S, WRAP_T, WRAP_R
    };

    enum class TextureParamValue {
        LINEAR, REPEAT, CLAMP_TO_EDGE
    };

    struct TextureParam {
        TextureParamName name;
        TextureParamValue value;
    };

    enum class TextureType : u32 {
        TEXTURE_2D = 0, CUBE_MAP = 1, TEXTURE_2D_MULTISAMPLE = 2
    };

    enum class TextureFaceType {
        FRONT, BACK,
        LEFT, RIGHT,
        TOP, BOTTOM
    };

    struct TextureFace {
        const char* filePath;
        TextureFaceType type;

        TextureFace(const char* filePath, const TextureFaceType& type)
        : filePath(filePath), type(type) {}
    };

    component(TextureComponent) {
        u32 textureId = 0;
        u32 typeId = 0;
        shader::IntUniform sampler = { "texture", 0 };

        TextureComponent() = default;

        TextureComponent(const u32& textureId, const u32& typeId, const shader::IntUniform& sampler)
        : textureId(textureId), sampler(sampler) {}

        TextureComponent(const u32& textureId, const u32& typeId, const s32& samplerSlot)
        : textureId(textureId), sampler({ "texture", samplerSlot }) {}
    };

    component(CubeMapTextureComponent) {
        u32 textureId = 0;
        u32 typeId = 0;
        shader::IntUniform sampler = { "cubeMap", 0 };

        CubeMapTextureComponent() = default;

        CubeMapTextureComponent(const u32& textureId, const u32& typeId)
        : textureId(textureId), typeId(typeId) {}

        CubeMapTextureComponent(const u32& textureId, const u32& typeId, const shader::IntUniform& sampler)
        : textureId(textureId), typeId(typeId), sampler(sampler) {}

        CubeMapTextureComponent(const u32& textureId, const u32& typeId, const s32& samplerSlot)
        : textureId(textureId), typeId(typeId), sampler({ "cubeMap", samplerSlot }) {}
    };

    using namespace shader;
    ShaderScript textureScript();
    ShaderScript textureUboScript();
    ShaderScript cubeMapTextureScript();
}
