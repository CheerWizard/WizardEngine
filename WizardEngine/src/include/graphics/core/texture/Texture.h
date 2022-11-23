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
    using namespace shader;

    // texture format for color buffer
    enum class ColorFormat {
        NONE = 0,
        RGBA8,
        RED_INTEGER,
        RGB16F, RGBA16F,
        RGB32F, RGBA32F
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

    struct ENGINE_API TextureParam {
        TextureParamName name;
        TextureParamValue value;
    };

    enum class TextureType : u32 {
        TEXTURE_2D = 0, CUBE_MAP = 1, TEXTURE_2D_MULTISAMPLE = 2, TEXTURE_2D_ARRAY = 3
    };

    enum class TextureFaceType {
        FRONT, BACK,
        LEFT, RIGHT,
        TOP, BOTTOM
    };

    struct ENGINE_API TextureFace {
        const char* filePath;
        TextureFaceType type;

        TextureFace(const char* filePath, const TextureFaceType& type)
        : filePath(filePath), type(type) {}
    };

    constexpr u32 invalidTextureId = 0;

    serialize_component(TextureComponent) {
        u32 textureId = invalidTextureId;
        u32 typeId = 0;
        IntUniform sampler = { "texture", 0 };

        TextureComponent() = default;

        TextureComponent(const u32& textureId, const u32& typeId, const shader::IntUniform& sampler)
        : textureId(textureId), sampler(sampler) {}

        TextureComponent(const u32& textureId, const u32& typeId, const s32& samplerSlot)
        : textureId(textureId), sampler({ "texture", samplerSlot }) {}

        TextureComponent(const u32& typeId, const shader::IntUniform& sampler)
        : typeId(typeId), sampler(sampler) {}

        TextureComponent(const IntUniform& sampler) : sampler(sampler) {}

        ENGINE_API void serialize(YAML::Emitter &out) override;
        ENGINE_API void deserialize(const YAML::Node &parent) override;

        inline bool isValid() const { return textureId != invalidTextureId; }
    };

    serialize_component(CubeMapTextureComponent) {
        u32 textureId = invalidTextureId;
        u32 typeId = 0;
        IntUniform sampler = { "cubeMap", 0 };

        CubeMapTextureComponent() = default;

        CubeMapTextureComponent(const u32& textureId, const u32& typeId)
        : textureId(textureId), typeId(typeId) {}

        CubeMapTextureComponent(const u32& textureId, const u32& typeId, const shader::IntUniform& sampler)
        : textureId(textureId), typeId(typeId), sampler(sampler) {}

        CubeMapTextureComponent(const u32& textureId, const u32& typeId, const s32& samplerSlot)
        : textureId(textureId), typeId(typeId), sampler({ "cubeMap", samplerSlot }) {}

        ENGINE_API void serialize(YAML::Emitter &out) override;
        ENGINE_API void deserialize(const YAML::Node &parent) override;

        inline bool isValid() const { return textureId != invalidTextureId; }
    };

    using namespace shader;
    ShaderScript textureScript();
    ShaderScript textureUboScript();
    ShaderScript cubeMapTextureScript();
}
