//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "stdint.h"

namespace engine {

    class Buffer {

    public:
        Buffer() {
            id = 0;
            capacity = 0;
        }
        virtual ~Buffer() = default;

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

    public:
        inline const uint32_t& getCapacity() const {
            return capacity;
        }

    protected:
        bool hasCapacity() const;

    protected:
        uint32_t id;
        uint32_t capacity;

    };

}