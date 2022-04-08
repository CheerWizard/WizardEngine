//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <graphics/core/texture/Texture.h>
#include <platform/graphics/TextureBuffer.h>

#include "string"

using namespace engine::shader;

namespace engine::graphics {

    struct MaterialComponent {
        const char* name = "material";
        Vec4fUniform color = { "color", {0.25, 0.5, 0.75, 1} };
        FloatUniform ambient = { "ambient", 0.1f };
        FloatUniform diffuse = { "diffuse", 0.5f };
        FloatUniform specular = { "specular", 0.5f };
        FloatUniform shiny = { "shiny", 32.0f };
        BoolUniform diffuseMapEnabled = { "diffuseMap", false };
        BoolUniform specularMapEnabled = { "specularMap", false };
    };

    struct MaterialMapsComponent {
        const char* name = "materialMaps";
        TextureComponent diffuse = {
                0,
                TextureBuffer::getTypeId(TextureType::TEXTURE_2D),
                IntUniform { "diffuse", 0 }
        };
        TextureComponent specular = {
                0,
                TextureBuffer::getTypeId(TextureType::TEXTURE_2D),
                IntUniform { "specular", 1 }
        };

        MaterialMapsComponent(const u32& diffuseId, const u32& specularId)
        : diffuse(diffuseId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D), { "diffuse", 0 }),
        specular(specularId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D), { "specular", 1 }) {}
    };

}