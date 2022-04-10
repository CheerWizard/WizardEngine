//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/ecs.h>

namespace engine::ecs {

    component(TagComponent) {
        std::string tag;
        TagComponent() = default;
        TagComponent(const std::string &tag) : tag(tag) {}
    };

}