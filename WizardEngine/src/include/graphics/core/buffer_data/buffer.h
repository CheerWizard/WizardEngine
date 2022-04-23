//
// Created by mecha on 15.03.2022.
//

#pragma once

#include <core/primitives.h>

namespace engine::graphics {

    using namespace core;

    class buffer {

    public:
        buffer() = default;
        buffer(const u32& totalCount) : totalCount(totalCount) {}
        ~buffer() = default;

    public:
        void setCount(const u32& count);
        void increaseCount(const u32& count);
        [[nodiscard]] bool hasCapacity(const u32& count) const;

    protected:
        u32 totalCount = 0;
        u32 count = 0;

    };

}
