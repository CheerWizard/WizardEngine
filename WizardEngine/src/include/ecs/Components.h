//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <core/uuid.h>
#include <ecs/ecs.h>

namespace engine::ecs {

    component(UUIDComponent) {
        uuid uuid;
        UUIDComponent() = default;
        UUIDComponent(const engine::uuid& uuid) : uuid(uuid) {}

        bool operator ==(const UUIDComponent& other) const { return uuid == other.uuid; }
    };

    component(TagComponent) {
        serializable()
        std::string tag;
        TagComponent() = default;
        TagComponent(const std::string &tag) : tag(tag) {}
    };

}