//
// Created by mecha on 25.03.2022.
//

#pragma once

#include <graphics/core/shader/BaseShader.h>

namespace engine {

    enum class TextureType : unsigned int {
        TEXTURE_2D = 0, CUBE_MAP = 1, TEXTURE_2D_MULTISAMPLE = 2
    };

    enum class TextureFaceType {
        FRONT, BACK,
        LEFT, RIGHT,
        TOP, BOTTOM
    };

    struct TextureFace {
        std::string fileName;
        TextureFaceType type;

        TextureFace(const std::string& fileName, const TextureFaceType& type)
        : fileName(fileName), type(type) {}
    };

    struct TextureComponent {
        std::string fileName;
        TextureType type = TextureType::TEXTURE_2D;
        shader::IntUniform sampler;

        TextureComponent(const std::string& fileName, const TextureType& type) : fileName(fileName), type(type) {}

        TextureComponent(const std::string& fileName, const TextureType& type, const shader::IntUniform& sampler)
        : fileName(fileName), type(type), sampler(sampler) {}

        TextureComponent(const shader::IntUniform& sampler) : sampler(sampler) {}

        TextureComponent(const TextureType& type, const shader::IntUniform& sampler) : type(type), sampler(sampler) {}
    };

    struct CubeMapTextureComponent {
        std::string name;
        std::vector<TextureFace> faces {};
        shader::IntUniform sampler = { "cubeMap", 5 };

        CubeMapTextureComponent(
                const std::string_view& name,
                const std::vector<TextureFace>& faces,
                const shader::IntUniform& sampler
        ) : name(name), faces(faces), sampler(sampler) {}

        CubeMapTextureComponent(
            const std::string_view& name,
            const std::vector<TextureFace>& faces
        ) : name(name), faces(faces) {}
    };

    using namespace shader;
    ShaderScript textureScript();
    ShaderScript textureUboScript();
    ShaderScript cubeMapTextureScript();
}
