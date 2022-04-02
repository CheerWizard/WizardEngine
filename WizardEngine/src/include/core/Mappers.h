//
// Created by mecha on 02.04.2022.
//

#pragma once

#include <vector>

namespace engine::core {

    template<typename T>
    T* map(const std::vector<T>& vector) {
        T* singleArray = new T[vector.size()];
        for (uint32_t i = 0 ; i < vector.size() ; i++) {
            singleArray[i] = vector[i];
        }
        return singleArray;
    }

}