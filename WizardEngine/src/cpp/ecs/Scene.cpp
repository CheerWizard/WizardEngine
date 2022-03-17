//
// Created by mecha on 26.09.2021.
//

#include <ecs/Scene.h>

namespace engine {

    Ref<Scene> copy(Ref<Scene>& scene) {
        auto copyScene = createRef<Scene>();
        copyScene->setTextureId(scene->getTextureId());
        return copyScene;
    }

}