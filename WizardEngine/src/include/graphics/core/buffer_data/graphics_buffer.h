//
// Created by mecha on 15.03.2022.
//

#pragma once

#include <core/primitives.h>
#include <core/core.h>

namespace engine::graphics {

    using namespace core;

    class ENGINE_API Buffer {

    public:
        Buffer() = default;
        Buffer(const u32& totalCount) : totalCount(totalCount) {}
        ~Buffer() = default;

    public:
        void setCount(const u32& count);
        void increaseCount(const u32& count);
        [[nodiscard]] bool hasCapacity(const u32& count) const;

    protected:
        u32 totalCount = 0;
        u32 count = 0;

    };

}
