//
// Created by mecha on 07.09.2022.
//

#pragma once

#include <rcc/RuntimeCompiler.h>

namespace engine::core {

    class ENGINE_API Game {

    public:
        virtual ~Game() = default;

    public:
        virtual void onCreate() = 0;
        virtual void onUpdate() = 0;
        virtual void onDestroy() = 0;
    };

}