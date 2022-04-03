//
// Created by mecha on 02.04.2022.
//

#pragma once

#define STL_DEQUE 1

#ifdef STL_DEQUE
// STL implementation
#include <deque>
namespace engine::core {
    template<typename T>
    using deque = std::deque<T>;
}
#else
// My implementation
namespace engine::core {
    template<typename T>
    class deque {};
}
#endif