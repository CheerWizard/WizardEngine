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

    serialize_component(TagComponent) {
        std::string tag;
        TagComponent() = default;
        TagComponent(const std::string &tag) : tag(tag) {}
        ~TagComponent() override = default;

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

}