//
// Created by mecha on 07.09.2021.
//

#include "Buffer.h"

#include "../../core/Logger.h"

namespace engine {

    bool Buffer::hasCapacity() const {
        if (capacity == 0) {
            ENGINE_WARN("Index buffer id={0} is not allocated!");
            return false;
        }
        return true;
    }

}