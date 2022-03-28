//
// Created by mecha on 28.03.2022.
//

#pragma once

#include <unordered_map>

namespace engine {

    class Fonts {

    private:
        Fonts() = default;

    public:
        Fonts(const Fonts &) = delete;

        Fonts &operator=(const Fonts &) = delete;

        Fonts(Fonts &&) = delete;

        Fonts &operator=(Fonts &&) = delete;

    public:
        static auto &get() {
            static Fonts instance;
            return instance;
        }
    };

}