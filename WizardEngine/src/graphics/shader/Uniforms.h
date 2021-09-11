//
// Created by mecha on 06.09.2021.
//

#pragma once

namespace engine {

    template<typename V>
    struct Uniform {
        char* name;
        V value;

        Uniform(char* name, const V& value) : name(name), value(value) {}
    };

}