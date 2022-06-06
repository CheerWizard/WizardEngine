//
// Created by mecha on 24.09.2021.
//

#include <graphics/core/buffer_data/IndexData.h>
#include <algorithm>

namespace engine::graphics {

    IndexData copy(const IndexData &indexData) {
        auto* copyIndices = new uint32_t[indexData.size];
        std::copy(indexData.values, indexData.values + indexData.size, copyIndices);

        return {
            copyIndices,
            indexData.offset,
            indexData.size
        };
    }

}