//
// Created by mecha on 17.05.2022.
//

#pragma once

#include <ecs/Scene.h>

#define SCENE_SERIALIZE_TEXT(scene, filepath) engine::io::SceneSerializer(scene).serializeText(filepath)
#define SCENE_SERIALIZE_BINARY(scene, filepath) engine::io::SceneSerializer(scene).serializeBinary(filepath)

#define SCENE_DESERIALIZE_TEXT(scene, filepath) engine::io::SceneSerializer(scene).deserializeText(filepath)
#define SCENE_DESERIALIZE_BINARY(scene, filepath) engine::io::SceneSerializer(scene).deserializeBinary(filepath)

namespace engine::io {

    class SceneSerializer {

    public:
        SceneSerializer(const Ref<ecs::Scene>& scene) : scene(scene) {}
        ~SceneSerializer() = default;

    public:
        void serializeText(const char* filepath);
        void serializeBinary(const char* filepath);

        bool deserializeText(const char* filepath);
        bool deserializeBinary(const char* filepath);

    private:
        Ref<ecs::Scene> scene;
    };

}