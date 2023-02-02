#pragma once

#include <core/core.h>
#include <core/primitives.h>
#include <cstdint>
#include <functional>

namespace engine::core {

    template<typename T>
    struct array {
        T* values = nullptr;
        u32 offset = 0;
        u32 size = 0;

        array() = default;
        array(T* values, u32 offset, u32 size)
        : values(values), size(size), offset(offset) {}

        void release();

        void forEach(const std::function<void(const T&)>& function);
        void forEach(const std::function<void(T&)>& function);
        void forEach(const std::function<void(T&, u32)>& function);

        array<T> copy();

        template<typename TO>
        array<TO> toVertexData(const std::function<TO(const T&)>& vertexMapper);
    };

    template<typename T>
    void array<T>::release() {
        delete[] values;
    }

    template<typename T>
    void array<T>::forEach(const std::function<void(const T&)>& function) {
        for (u32 i = 0; i < size; i++) {
            function(values[i]);
        }
    }

    template<typename T>
    void array<T>::forEach(const std::function<void(T&)>& function) {
        for (u32 i = 0; i < size; i++) {
            function(values[i]);
        }
    }

    template<typename T>
    void array<T>::forEach(const std::function<void(T&, u32)> &function) {
        for (u32 i = 0; i < size; i++) {
            function(values[i], i);
        }
    }

    template<typename T>
    array<T> array<T>::copy() {
        auto* copyVertices = new T[size];
        std::copy(values, values + size, copyVertices);

        return array<T> {
                copyVertices,
                offset,
                size
        };
    }

    template<typename T>
    template<typename TO>
    array<TO> array<T>::toVertexData(const std::function<TO(const T &)>& vertexMapper) {
        auto* fromVertices = values;
        auto* toVertices = new TO[size];
        for (auto i = 0; i < size; i++) {
            toVertices[i] = vertexMapper(fromVertices[i]);
        }
        return array<TO> {
                toVertices,
                offset,
                size
        };
    }

    template<typename FROM, typename TO>
    array<TO> toVertexData(const std::vector<FROM>& inVertices) {
        size_t size = inVertices.size();
        auto* outVertices = new TO[size];
        for (auto i = 0; i < size; i++) {
            outVertices[i] = { inVertices[i] };
        }
        return array<TO> {
                outVertices,
                0,
                static_cast<uint32_t>(size)
        };
    }
}