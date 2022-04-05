//
// Created by mecha on 24.09.2021.
//

#pragma once

#include <cstdint>
#include <algorithm>

#define MIN_INDEX_COUNT 3

namespace engine::graphics {

    struct IndexData {
        uint32_t* indices = nullptr;
        uint32_t indexStart = 0;
        uint32_t indexCount = MIN_INDEX_COUNT;
    };

    IndexData copy(const IndexData &indexData);

}