//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../../math/Uniform.h"

namespace engine {

    struct Texture : IntUniform {
        Texture(const char* name, const int &slot) : IntUniform(name, slot) {}
    };

    struct TextureComponent {
        Texture texture;

        TextureComponent(const char* name, const int &slot) : texture(Texture(name, slot)) {}
        TextureComponent(const Texture &texture) : texture(texture) {}

    public:
        void applyChanges() {
            texture.applyChanges();
        }
    };

}