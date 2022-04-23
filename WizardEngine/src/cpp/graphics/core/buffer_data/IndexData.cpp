//
// Created by mecha on 24.09.2021.
//

#include <graphics/core/buffer_data/IndexData.h>
#include <algorithm>

namespace engine::graphics {

    IndexData copy(const IndexData &indexData) {
        auto* copyIndices = new uint32_t[indexData.indexCount];
        std::copy(indexData.indices, indexData.indices + indexData.indexCount, copyIndices);

        return {
            copyIndices,
            indexData.indexStart,
            indexData.indexCount
        };
    }

}