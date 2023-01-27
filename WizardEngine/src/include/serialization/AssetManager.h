//
// Created by mecha on 04.06.2022.
//

#pragma once

#include <serialization/SceneSerializer.h>

namespace engine::io {

    using namespace ecs;

    class ENGINE_API LocalAssetManager final {

    public:
        static const Ref<Scene>& read(const char* filepath);
        static const Ref<Scene>& readBin(const char* filepath);
        static void readAll(const char* dirPath);
        static void write(const Ref<Scene>& scene, const char* filepath);
        static void writeBin(const Ref<Scene>& scene, const char* filepath);
        static void writeAll(const char* dirPath);
        static void addScene(const Ref<Scene>& scene);
        static void addScenes(const vector<Ref<Scene>>& scenes);
        static void removeScene(const uuid& sceneId);
        static void clear();
        static void setScene(const Ref<Scene>& scene);
        static const Ref<Scene>& getScene(const uuid& sceneId);
        static const std::unordered_map<uuid, Ref<Scene>>& getScenes();
        static size_t getSceneSize();

    private:
        static std::unordered_map<uuid, Ref<Scene>> s_Scenes;
    };

    class ENGINE_API RemoteAssetManager final {

    public:
        static void saveScene(const Ref<Scene>& scene);
        static void loadScene(const char* sceneName);
        static void dispatchScene(Ref<Scene>& scene, const YAML::Node &gdNode, u32 headerType);

    private:
        static void onSceneSaved(const YAML::Node& gdNode);
        static void onSceneLoaded(const YAML::Node& gdNode, Ref<Scene>& scene);
    };

}