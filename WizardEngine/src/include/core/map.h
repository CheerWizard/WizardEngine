//
// Created by mecha on 03.04.2022.
//

#pragma once

#define STL_MAP 1

#ifdef STL_MAP
// STL implementation
#include <map>
namespace engine::core {
    template<typename K, typename V>
    using map = std::map<K,V>;
}
#else
// My implementation
namespace engine::core {
    template<typename K, typename V>
    class map {};
}
#endif