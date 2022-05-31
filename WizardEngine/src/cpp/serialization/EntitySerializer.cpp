//
// Created by mecha on 21.05.2022.
//

#include <serialization/EntitySerializer.h>
#include <core/filesystem.h>

#include <ecs/Components.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::io {

    void EntitySerializable::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap; // Entity
        yaml::serialize(out, "Entity", entity.getId());
        serializeComponents(out);
        out << YAML::EndMap; // Entity
    }

    void EntitySerializable::deserialize(const YAML::Node &parent) {
//        entity.setId(parent["Entity"].as<u32>());
        deserialize<ecs::TagComponent>(parent);
        deserialize<graphics::Transform2dComponent>(parent);
        deserialize<graphics::Transform3dComponent>(parent);
        deserialize<graphics::Camera2dComponent>(parent);
        deserialize<graphics::Camera3dComponent>(parent);
    }

    void EntitySerializable::serializeComponents(YAML::Emitter &out) {
        serialize<ecs::TagComponent>(out);
        serialize<graphics::Transform2dComponent>(out);
        serialize<graphics::Transform3dComponent>(out);
        serialize<graphics::Camera2dComponent>(out);
        serialize<graphics::Camera3dComponent>(out);
    }

    void EntitySerializer::serializeText(YAML::Emitter &out) {
        entity.serialize(out);
    }

    void EntitySerializer::serializeBinary(const char *filepath) {
    }

    void EntitySerializer::deserializeText(const YAML::Node &entityNode) {
        entity.deserialize(entityNode);
    }

    void EntitySerializer::deserializeBinary(const char *filepath) {
    }

    const char *EntitySerializer::serializeText() {
        YAML::Emitter out;
        serializeText(out);
        return strdup(out.c_str());
    }

    void EntitySerializer::serializeTextFile(const char *filepath) {
        engine::filesystem::write(filepath, serializeText());
    }

    void EntitySerializer::deserializeText(const char *entityText) {
        YAML::Node entityNode;
        try {
            entityNode = YAML::Load(entityText);
            deserializeText(entityNode);
        } catch (YAML::ParserException& e) {
            ENGINE_ERR("EntitySerializer: Failed to parse YAML text!");
            ENGINE_ERR(e.msg);
            return;
        }
    }

    void EntitySerializer::deserializeTextFile(const char *filepath) {
        YAML::Node entityNode;
        try {
            entityNode = YAML::LoadFile(filepath);
            deserializeText(entityNode);
        } catch (YAML::ParserException& e) {
            ENGINE_ERR("EntitySerializer: Failed to parse YAML text file!");
            ENGINE_ERR(e.msg);
            return;
        }
    }
}