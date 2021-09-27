//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "string"

namespace engine {

    struct TagComponent {
        std::string tag;
        TagComponent() = default;
        TagComponent(const std::string &tag) : tag(tag) {}
    };

}