//
// Created by mecha on 17.05.2022.
//

#pragma once

#include <ecs/Scene.h>

#define SCENE_SERIALIZE_TEXT(scene) engine::io::SceneSerializer(scene).serializeText()
#define SCENE_SERIALIZE_BINARY(scene, filepath) engine::io::SceneSerializer(scene).serializeBinary(filepath)

#define SCENE_DESERIALIZE_TEXT(scene, data) engine::io::SceneSerializer(scene).deserializeText(data)
#define SCENE_DESERIALIZE_BINARY(scene, filepath) engine::io::SceneSerializer(scene).deserializeBinary(filepath)

#define SCENE_SERIALIZE_TEXT_FILE(scene, filepath) engine::io::SceneSerializer(scene).serializeTextFile(filepath)
#define SCENE_DESERIALIZE_TEXT_FILE(scene, filepath) engine::io::SceneSerializer(scene).deserializeTextFile(filepath)

namespace engine::io {

    class SceneSerializable : public Serializable {

    public:
        SceneSerializable(const Ref<ecs::Scene>& scene) : scene(scene) {}
        ~SceneSerializable() override = default;

    public:
        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;

    private:
        Ref<ecs::Scene> scene;
    };

    class SceneSerializer {

    public:
        SceneSerializer(const Ref<ecs::Scene>& scene) : scene(scene) {}

    public:
        const char* serializeText();
        void serializeBinary(const char* filepath);
        void serializeTextFile(const char* filepath);

        bool deserializeText(const char* data);
        bool deserializeBinary(const char* filepath);
        bool deserializeTextFile(const char* filepath);

    private:
        bool deserialize(const YAML::Node& sceneNode);

    private:
        SceneSerializable scene;
    };

}