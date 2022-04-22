//
// Created by mecha on 24.09.2021.
//

#pragma once

#include <core/primitives.h>

#define MIN_INDEX_COUNT 3

namespace engine::graphics {

    using namespace core;

    struct IndexData {
        u32* indices = nullptr;
        u32 indexStart = 0;
        u32 indexCount = MIN_INDEX_COUNT;
    };

    IndexData copy(const IndexData &indexData);

}