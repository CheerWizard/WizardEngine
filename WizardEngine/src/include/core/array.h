//
// Created by mecha on 06.06.2022.
//

#pragma once

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

}