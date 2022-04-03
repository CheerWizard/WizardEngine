//
// Created by mecha on 02.04.2022.
//

#pragma once

#define STL_VECTOR 1

#ifdef STL_VECTOR
// STL implementation
#include <vector>
namespace engine::core {
    template<typename T>
    using vector = std::vector<T>;
}
#else
// My implementation
namespace engine::core {
    template<typename T>
    class vector {};
}
#endif