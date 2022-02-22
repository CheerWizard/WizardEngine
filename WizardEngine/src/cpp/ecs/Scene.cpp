//
// Created by mecha on 26.09.2021.
//

#include <ecs/Scene.h>

namespace engine {

    void Scene::clear() {
        EntityContainer::clear();
        _families.clear();
    }

    void Scene::addFamily(const Family &family) {
//        _families.emplace_back(family);
    }

    void Scene::removeFamily(const Family &family) {
        auto it = std::find(_families.begin(), _families.end(), family);
//        _families.erase(it);
    }

    bool Scene::isEmptyFamilies() {
        return _families.empty();
    }

    bool Scene::isEmpty() {
        return EntityContainer::isEmpty() && isEmptyFamilies();
    }

    Ref<Scene> copy(Ref<Scene>& scene) {
        auto copyScene = createRef<Scene>();
        copyScene->setTextureId(scene->getTextureId());
        // todo currently not working, because unable to copy family
//        copyScene->setFamilies(scene->getFamilies());
        return copyScene;
    }

}