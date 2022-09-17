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

    class ENGINE_API EntitySerializable : public Serializable {

    public:
        EntitySerializable(const ecs::Entity& entity) : entity(entity) {}

    public:
        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;

    private:
        void serializeComponents(YAML::Emitter& out);
        template<typename T>
        void deserializeComponent(const YAML::Node &entityNode);
        template<typename T>
        void serializeComponent(YAML::Emitter& out);

    private:
        ecs::Entity entity;
    };

    template<typename T>
    void EntitySerializable::deserializeComponent(const YAML::Node &entityNode) {
        // todo works only if entity has no T component yet
        entity.add<T>(deserialize_from(T, entityNode));
    }

    template<typename T>
    void EntitySerializable::serializeComponent(YAML::Emitter &out) {
        T* component = entity.template get<T>();
        if (component) {
            component->serialize(out);
        }
    }

    class ENGINE_API EntitySerializer final {

    public:
        EntitySerializer(const ecs::Entity& entity) : entity(entity) {}

    public:
        void serializeText(YAML::Emitter& out);
        void serializeBinary(const char* filepath);
        const char* serializeText();
        void serializeTextFile(const char* filepath);

        void deserializeText(const YAML::Node& entityNode);
        void deserializeBinary(const char* filepath);
        void deserializeText(const char* entityText);
        bool deserializeTextFile(const char* filepath);

    private:
        EntitySerializable entity;
    };

}
