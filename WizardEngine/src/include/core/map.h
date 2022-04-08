//
// Created by mecha on 03.04.2022.
//

#pragma once

#define STL_MAP 1

#ifdef STL_MAP
// STL implementation
#include <map>
#include <unordered_map>
namespace engine::core {
    template<typename K, typename V>
    using map = std::map<K,V>;
    template<typename K, typename V>
    using unordered_map = std::unordered_map<K,V>;
}
#else
// My implementation
namespace engine::core {
    template<typename K, typename V>
    class map {};
    template<typename K, typename V>
    class unordered_map {};
}
#endif