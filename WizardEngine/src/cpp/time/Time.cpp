//
// Created by mecha on 03.09.2022.
//

#include <time/Time.h>

namespace engine::time {

    float Time::milliseconds() const {
        return ms;
    }

    float Time::seconds() const {
        return ms / 1000;
    }

}