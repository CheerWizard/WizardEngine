//
// Created by mecha on 15.03.2022.
//

#include <graphics/core/buffer_data/buffer.h>

namespace engine::graphics {

    void buffer::setCount(const uint32_t &count) {
        this->count = count;
    }

    void buffer::increaseCount(const uint32_t &count) {
        this->count += count;
    }

    bool buffer::hasCapacity(const uint32_t &count) const {
        return this->count + count <= totalCount;
    }

}