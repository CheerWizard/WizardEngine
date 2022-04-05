//
// Created by mecha on 15.03.2022.
//

#pragma once

#include <core/primitives.h>

namespace engine::graphics {

    using namespace core;

    class Buffer {

    public:
        Buffer(const u32& totalCount) : totalCount(totalCount) {}
        ~Buffer() = default;

    public:
        void setCount(const u32& count);
        void increaseCount(const u32& count);
        [[nodiscard]] bool hasCapacity(const u32& count) const;

    protected:
        u32 totalCount;
        u32 count = 0;

    };

}
