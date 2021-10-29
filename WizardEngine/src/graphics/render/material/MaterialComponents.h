//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../../shader/Uniform.h"

namespace engine {

    struct TextureComponent : IntUniform {
        TextureComponent(const char* name, const int &slot) : IntUniform(name, slot) {
            applyChanges();
        }
    };

}