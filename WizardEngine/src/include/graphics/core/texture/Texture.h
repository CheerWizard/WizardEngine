//
// Created by mecha on 25.03.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>

namespace engine::graphics {

    using namespace core;
    using namespace shader;

    // texture format for color buffer
    struct ENGINE_API ColorFormat final {
        static u32 NONE;
        static int RED_I32;
        static u32 RED_INTEGER;
        static u32 GREEN_INTEGER;
        static u32 BLUE_INTEGER;
        static int RED;
        static int GREEN;
        static int BLUE;
        static u32 U_RED;
        static u32 U_GREEN;
        static u32 U_BLUE;
        static u32 RGB;
        static u32 RGBA;
        static int RGB8;
        static int RGBA8;
        static int R16F;
        static int RGB16F;
        static int RGBA16F;
        static int RGB32F;
        static int RGBA32F;
        static int SRGB;
        static int SRGBA;
    };

    // color pixels type
    struct ENGINE_API PixelsType final {
        static u32 U_BYTE;
        static u32 FLOAT;
        static u32 INT;
    };

    // texture format for depth and stencil buffers
    struct ENGINE_API DepthStencilFormat final {
        static u32 NONE;
        static u32 DEPTH16;
        static u32 DEPTH24STENCIL8;
        static u32 DEPTH24;
        static u32 DEPTH32;
        static u32 DEPTH32STENCIL8;
    };

    // texture filter names
    struct ENGINE_API TextureParamName final {
        static u32 MIN_FILTER;
        static u32 MAG_FILTER;
        static u32 WRAP_S;
        static u32 WRAP_T;
        static u32 WRAP_R;
    };

    // texture filter values
    struct ENGINE_API TextureParamValue final {
        static int LINEAR;
        static int REPEAT;
        static int CLAMP_TO_EDGE;
    };

    struct ENGINE_API TextureParam {
        u32 name;
        int value;
    };

    // texture types
    struct ENGINE_API TextureType final {
        static u32 TEXTURE_2D;
        static u32 CUBE_MAP;
        static u32 TEXTURE_2D_MULTISAMPLE;
        static u32 TEXTURE_2D_ARRAY;
    };

    // texture face types
    struct ENGINE_API TextureFaceType final {
        static u32 FRONT;
        static u32 BACK;
        static u32 LEFT;
        static u32 RIGHT;
        static u32 TOP;
        static u32 BOTTOM;
    };

    struct ENGINE_API TextureFace {
        const char* filePath;
        u32 type;

        TextureFace(const char* filePath, u32 type)
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
