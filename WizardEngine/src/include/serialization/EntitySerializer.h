//
// Created by mecha on 21.05.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <yaml/yaml.h>

#define ENTITY_SERIALIZE_TEXT(entity, emitter) engine::io::EntitySerializer(entity).serializeText(emitter)
#define ENTITY_SERIALIZE_BINARY(entity, filepath) engine::io::EntitySerializer(entity).serializeBinary(filepath)

#define ENTITY_DESERIALIZE_TEXT(entity, node) engine::io::EntitySerializer(entity).deserializeText(node)
#define ENTITY_DESERIALIZE_BINARY(entity, filepath) engine::io::EntitySerializer(entity).deserializeBinary(filepath)

namespace engine::io {

    class EntitySerializer final {

    public:
        EntitySerializer(const ecs::Entity& entity) : entity(entity) {}
        ~EntitySerializer() = default;

    public:
        void serializeText(YAML::Emitter& out);
        void serializeBinary(const char* filepath);
        const char* serializeText();
        void serializeTextFile(const char* filepath);

        void deserializeText(const YAML::Node& entityNode);
        void deserializeBinary(const char* filepath);
        void deserializeText(const char* entityText);
        void deserializeTextFile(const char* filepath);

    private:
        void serializeComponents(YAML::Emitter& out);

    private:
        ecs::Entity entity;
    };

}
