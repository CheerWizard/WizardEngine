//
// Created by mecha on 17.05.2022.
//

#include <serialization/SceneSerializer.h>
#include <serialization/EntitySerializer.h>

#include <core/filesystem.h>

namespace engine::io {

    void SceneSerializable::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;

        out << YAML::Key << "Scene" << YAML::Value << scene->getName();
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        scene->getRegistry().eachEntity([&](ecs::entity_id entityId) {
            ENTITY_SERIALIZE_TEXT(ecs::Entity(scene.get(), entityId), out);
        });
        out << YAML::EndSeq;

        out << YAML::EndMap;
    }

    void SceneSerializable::deserialize(const YAML::Node &parent) {
        auto sceneName = parent["Scene"].as<std::string>();
        ENGINE_TRACE("Scene deserialized '{0}'", sceneName);

        auto entitiesNode = parent["Entities"];
        if (entitiesNode) {
            for (auto entityNode : entitiesNode) {
                ENTITY_DESERIALIZE_TEXT(ecs::Entity(scene.get()), entityNode);
            }
        }
    }

    const char* SceneSerializer::serializeText() {
        YAML::Emitter emitter;
        scene.serialize(emitter);
        return emitter.c_str();
    }

    void SceneSerializer::serializeBinary(const char *filepath) {
    }

    void SceneSerializer::serializeTextFile(const char *filepath) {
        filesystem::write(filepath, serializeText());
    }

    bool SceneSerializer::deserializeText(const char *data) {
        YAML::Node sceneNode;
        try {
            sceneNode = YAML::Load(data);
        } catch (YAML::ParserException& e) {
            ENGINE_ERR("SceneSerializer: Failed to parse YAML text!");
            ENGINE_ERR(e.msg);
            return false;
        }

        return deserialize(sceneNode);
    }

    bool SceneSerializer::deserializeTextFile(const char *filepath) {
        YAML::Node sceneNode;
        try {
            sceneNode = YAML::LoadFile(filepath);
        } catch (YAML::ParserException& e) {
            ENGINE_ERR("SceneSerializer: Failed to parse YAML text file!");
            ENGINE_ERR(e.msg);
            return false;
        }

        return deserialize(sceneNode);
    }

    bool SceneSerializer::deserialize(const YAML::Node &sceneNode) {
        scene.deserialize(sceneNode);
        return true;
    }

    bool SceneSerializer::deserializeBinary(const char *filepath) {
        return false;
    }
}