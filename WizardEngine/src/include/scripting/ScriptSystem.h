//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <ecs/Scene.h>
#include <scripting/ScriptComponents.h>

namespace engine::scripting {

    using namespace core;

    class ENGINE_API ScriptSystem final {

    public:
        static void onCreate();
        static void onUpdate(time::Time dt);
        static void onDestroy();
        static Ref<Scene> activeScene;
    };

}

