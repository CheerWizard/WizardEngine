//
// Created by mecha on 21.05.2022.
//

#pragma once

#define STL_QUEUE 1

#ifdef STL_QUEUE
// STL implementation
#include <queue>
namespace engine::core {
    template<typename T>
    using queue = std::queue<T>;
}
#else
// My implementation
namespace engine::core {
    template<typename T>
    class queue {};
}
#endif