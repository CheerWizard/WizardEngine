//
// Created by mecha on 04.06.2022.
//

#include <serialization/AssetManager.h>
#include <network/network_client.h>
#include <core/filesystem.h>

namespace engine::io {

    std::unordered_map<uuid, Ref<Scene>> LocalAssetManager::s_Scenes;

    const Ref<Scene>& LocalAssetManager::read(const char *filepath) {
        Ref<Scene> newScene = createRef<Scene>(engine::filesystem::getFileName(std::string(filepath)));
        ENGINE_INFO("LocalAssetManager: read '{0}'", filepath);
        SCENE_DESERIALIZE_TEXT_FILE(newScene, filepath);
        s_Scenes[newScene->getId()] = newScene;
        return s_Scenes.at(newScene->getId());
    }

    const Ref<Scene>& LocalAssetManager::readBin(const char *filepath) {
        Ref<Scene> newScene = createRef<Scene>(engine::filesystem::getFileName(std::string(filepath)));
        ENGINE_INFO("LocalAssetManager: read '{0}'", filepath);
        SCENE_DESERIALIZE_TEXT_FILE(newScene, filepath);
        s_Scenes[newScene->getId()] = newScene;
        return s_Scenes.at(newScene->getId());
    }

    void LocalAssetManager::write(const Ref<Scene>& scene, const char *filepath) {
        ENGINE_INFO("LocalAssetManager: write '{0}'", filepath);
        SCENE_SERIALIZE_TEXT_FILE(scene, filepath);
    }

    void LocalAssetManager::writeBin(const Ref<Scene>& scene, const char *filepath) {
        ENGINE_INFO("LocalAssetManager: write '{0}'", filepath);
        SCENE_SERIALIZE_TEXT_FILE(scene, filepath);
    }

    void LocalAssetManager::addScene(const Ref<Scene>& scene) {
        s_Scenes[scene->getId()] = scene;
    }

    void LocalAssetManager::removeScene(const uuid& sceneId) {
        s_Scenes.erase(sceneId);
    }

    void LocalAssetManager::clear() {
        s_Scenes.clear();
    }

    void LocalAssetManager::addScenes(const vector<Ref<Scene>>& newScenes) {
        for (const auto& newScene : newScenes) {
            s_Scenes[newScene->getId()] = newScene;
        }
    }

    void LocalAssetManager::setScene(const Ref<Scene>& scene) {
        s_Scenes[scene->getId()] = scene;
    }

    const Ref<Scene>& LocalAssetManager::getScene(const uuid& sceneId) {
        return s_Scenes.at(sceneId);
    }

    void LocalAssetManager::readAll(const char *dirPath) {
        ENGINE_INFO("LocalAssetManager: readAll '{0}'", dirPath);

        if (!engine::filesystem::exists(dirPath)) {
            engine::filesystem::newDirectory(dirPath);
            return;
        }

        for (auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_directory())
                continue;

            const auto& path = entry.path();
            auto extension = path.extension();
            std::string scenePath = path.string();
            ENGINE_INFO("LocalAssetManager: readAll '{0}'", scenePath.c_str());

            if (extension == ".scene") {
                // todo read all binary data into scene, instead of .yaml
                readBin(scenePath.c_str());
            } else if (extension == ".yaml") {
                read(scenePath.c_str());
            } else
                continue;
        }
    }

    void LocalAssetManager::writeAll(const char* dirPath) {
        ENGINE_INFO("LocalAssetManager: writeAll '{0}'", dirPath);

        if (!engine::filesystem::exists(dirPath)) {
            engine::filesystem::newDirectory(dirPath);
            return;
        }

        for (const auto& entry : s_Scenes) {
            std::stringstream ss(dirPath);
            ss << "/" << entry.second->getName() << ".yaml";
            std::string scenePath = ss.str();
            ENGINE_INFO("LocalAssetManager: writeAll '{0}'", scenePath.c_str());
            write(entry.second, scenePath.c_str());
        }
    }

    const std::unordered_map<uuid, Ref<Scene>> &LocalAssetManager::getScenes() {
        return s_Scenes;
    }

    size_t LocalAssetManager::getSceneSize() {
        return s_Scenes.size();
    }

    using namespace network;

    void RemoteAssetManager::saveScene(const Ref<Scene> &scene) {
        ENGINE_INFO("RemoteAssetManager: saveScene '{0}'", scene->getName().c_str());
        GDHeader header(CLIENT_TO_SERVER, SERVER_SAVE_SCENE);
        SceneSerializable body(scene);
        tcp::Client::get().getRequestQueue().push(header, body);
    }

    void RemoteAssetManager::loadScene(const char* sceneName) {
        ENGINE_INFO("RemoteAssetManager: loadScene '{0}'", sceneName);
        GDHeader header(CLIENT_TO_SERVER, SERVER_LOAD_SCENE);
        GDString body(sceneName);
        tcp::Client::get().getRequestQueue().push(header, body);
    }

    void RemoteAssetManager::dispatchScene(Ref<Scene> &scene, const YAML::Node &gdNode, u32 headerType) {
        switch (headerType) {
            case SERVER_SAVE_SCENE:
                onSceneSaved(gdNode);
                break;
            case SERVER_LOAD_SCENE:
                onSceneLoaded(gdNode, scene);
                break;
            default:
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