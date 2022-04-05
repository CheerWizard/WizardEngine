//
// Created by mecha on 15.03.2022.
//

#pragma once

#include <cstdint>

namespace engine::graphics {

    class Buffer {

    public:
        Buffer(const uint32_t& totalCount) : totalCount(totalCount) {}
        ~Buffer() = default;

    public:
        void setCount(const uint32_t& count);
        void increaseCount(const uint32_t& count);
        [[nodiscard]] bool hasCapacity(const uint32_t& count) const;

    protected:
        uint32_t totalCount;
        uint32_t count = 0;

    };

}
