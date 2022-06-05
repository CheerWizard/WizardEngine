//
// Created by mecha on 04.06.2022.
//

#include <serialization/AssetManager.h>
#include <network/network_client.h>

namespace engine::io {

    void LocalAssetManager::saveScene(const Ref<Scene> &scene) {
        std::stringstream sceneFilePath;
        sceneFilePath << "assets/scenes/" << scene->getName() << ".yaml";
        ENGINE_INFO("LocalAssetManager: saveScene name='{0}' filepath='{1}'", scene->getName(), sceneFilePath.str());
        SCENE_SERIALIZE_TEXT_FILE(scene, sceneFilePath.str().c_str());
    }

    void LocalAssetManager::loadScene(Ref<Scene> &sceneOut) {
        std::stringstream sceneFilePath;
        sceneFilePath << "assets/scenes/" << sceneOut->getName() << ".yaml";
        ENGINE_INFO("LocalAssetManager: loadScene '{0}'", sceneFilePath.str());
        SCENE_DESERIALIZE_TEXT_FILE(sceneOut, sceneFilePath.str().c_str());
    }

    Ref<Scene> LocalAssetManager::loadScene(const char *sceneName) {
        auto newScene = createRef<Scene>(sceneName);
        loadScene(newScene);
        return newScene;
    }

    using namespace network;

    void RemoteAssetManager::saveScene(const Ref<Scene> &scene) {
        ENGINE_INFO("RemoteAssetManager: saveScene '{0}'", scene->getName());
        GDHeader header(CLIENT_TO_SERVER, SERVER_SAVE_SCENE);
        SceneSerializable body(scene);
        udp::Client::getRequestQueue().push(header, body);
    }

    void RemoteAssetManager::loadScene(const char* sceneName) {
        ENGINE_INFO("RemoteAssetManager: loadScene '{0}'", sceneName);
        GDHeader header(CLIENT_TO_SERVER, SERVER_LOAD_SCENE);
        GDString body(sceneName);
        udp::Client::getRequestQueue().push(header, body);
    }

    void RemoteAssetManager::dispatchScene(Ref<Scene> &scene, const YAML::Node &gdNode, u32 headerType) {
        switch (headerType) {
            case SERVER_SAVE_SCENE:
                onSceneSaved(gdNode);
                break;
            case SERVER_LOAD_SCENE:
                onSceneLoaded(gdNode, scene);
                break;
        }
    }

    void RemoteAssetManager::onSceneSaved(const YAML::Node &gdNode) {
        ENGINE_INFO("RemoteAssetManager: onSceneSaved()");
        GDResponse response;
        response.deserialize(gdNode);
        switch (response.statusCode) {
            case GD_CODE_200:
                ENGINE_INFO("RemoteAssetManager: Scene saved!");
                break;
            case GD_CODE_404:
                ENGINE_ERR("RemoteAssetManager: Error during saving of scene \nstatus code: {0} \nmessage: {1}",
                           response.statusCode, response.message);
                break;
        }
    }

    void RemoteAssetManager::onSceneLoaded(const YAML::Node &gdNode, Ref<Scene>& scene) {
        ENGINE_INFO("RemoteAssetManager: onSceneLoaded()");
        SceneSerializable body(scene);
        body.deserialize(gdNode);
    }
}