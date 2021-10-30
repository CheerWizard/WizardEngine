//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../../shader/Uniform.h"

#include "string"

namespace engine {

    struct TextureComponent : IntUniform {
        std::string fileName;
        TextureComponent(const std::string &fileName, const char* name, const int &slot)
        : IntUniform(name, slot), fileName(fileName) {
            applyChanges();
        }
    };

}