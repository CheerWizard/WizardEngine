//
// Created by mecha on 24.10.2021.
//

#include "Family.h"
#include "Components.h"

namespace engine {

    void Family::create(const std::string &tag) {
        container = this;
        id = createEntityId();
        add<TagComponent>(tag);
    }

}