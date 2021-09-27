//
// Created by mecha on 25.09.2021.
//

#pragma once

#include <cstdint>
#include <string>

#include "../../math/Uniform.h"

namespace engine {

    struct TextureData {
        int width;
        int height;
        int channels;
        void* data;

        TextureData(const int &width,
                    const int &height,
                    const int &channels,
                    void* data) :
                    width(width),
                    height(height),
                    channels(channels),
                    data(data) {}
    };

}