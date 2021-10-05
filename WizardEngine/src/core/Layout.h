//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Time.h"

namespace engine {

    class Layout {
    public:
        virtual void onUpdate(Time dt) = 0;
    };

}