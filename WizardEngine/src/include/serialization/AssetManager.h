//
// Created by mecha on 04.06.2022.
//

#pragma once

#include <serialization/SceneSerializer.h>

namespace engine::io {

    using namespace ecs;

    class ENGINE_API LocalAssetManager final {

    public:
        static void saveScene(const Ref<Scene>& scene);
        static void loadScene(Ref<Scene>& sceneOut);
        static Ref<Scene> loadScene(const char* sceneName);
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