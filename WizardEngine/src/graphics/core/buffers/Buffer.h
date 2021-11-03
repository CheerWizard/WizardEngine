//
// Created by mecha on 07.09.2021.
//

#pragma once

#include <cstdint>

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
        virtual void recreate() = 0;

    public:
        inline const uint32_t& getCapacity() const {
            return capacity;
        }

        inline const uint32_t& getId() const {
            return id;
        }

        inline void setId(const uint32_t &id) {
            this->id = id;
        }

        inline void setCapacity(const uint32_t capacity) {
            this->capacity = capacity;
        }

    protected:
        bool hasCapacity() const;

    protected:
        uint32_t id;
        uint32_t capacity;

    };

}