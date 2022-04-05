//
// Created by mecha on 15.03.2022.
//

#include <graphics/core/buffer_data/Buffer.h>

namespace engine::graphics {

    void Buffer::setCount(const uint32_t &count) {
        this->count = count;
    }

    void Buffer::increaseCount(const uint32_t &count) {
        this->count += count;
    }

    bool Buffer::hasCapacity(const uint32_t &count) const {
        return this->count + count <= totalCount;
    }

}