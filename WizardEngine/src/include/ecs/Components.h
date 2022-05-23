//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/ecs.h>

namespace engine::ecs {

    serialize_component(TagComponent) {
        std::string tag;
        TagComponent() = default;
        TagComponent(const std::string &tag) : tag(tag) {}

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

}