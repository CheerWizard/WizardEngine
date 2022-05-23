//
// Created by mecha on 21.05.2022.
//

#include <serialization/EntitySerializer.h>
#include <core/filesystem.h>

#include <ecs/Components.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::io {

    void EntitySerializer::serializeText(YAML::Emitter &out) {
        out << YAML::BeginMap; // Entity
        yaml::serialize(out, "Entity", entity.getId());
        serializeComponents(out);
        out << YAML::EndMap; // Entity
    }

    void EntitySerializer::serializeBinary(const char *filepath) {
    }

    void EntitySerializer::deserializeText(const YAML::Node &entityNode) {
        entity.deserialize<ecs::TagComponent>(entityNode);
        entity.deserialize<graphics::Transform2dComponent>(entityNode);
        entity.deserialize<graphics::Transform3dComponent>(entityNode);
        entity.deserialize<graphics::Camera2dComponent>(entityNode);
    }

    void EntitySerializer::deserializeBinary(const char *filepath) {
    }

    void EntitySerializer::serializeComponents(YAML::Emitter &out) {
        entity.serialize<ecs::TagComponent>(out);
        entity.serialize<graphics::Transform2dComponent>(out);
        entity.serialize<graphics::Transform3dComponent>(out);
        entity.serialize<graphics::Camera2dComponent>(out);
    }

    const char *EntitySerializer::serializeText() {
        YAML::Emitter out;
        serializeText(out);
        return out.c_str();
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