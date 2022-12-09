//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <scripting/Scriptable.h>
#include <ecs/Scene.h>

namespace engine::scripting {

    using namespace ecs;

    component(NativeScript) {
        std::function<void()> onCreateFunction;
        std::function<void()> onDestroyFunction;
        std::function<void(time::Time)> onUpdateFunction;

        template<class T>
        static NativeScript create(const Ref<Scene>& scene) {
            NativeScript script;
            script.bind<T>(scene);
            return script;
        }

        template<class T>
        void bind(const Ref<Scene>& scene) {
            T scriptable(scene.get());
            onCreateFunction = [&scriptable]() { scriptable.onCreate(); };
            onDestroyFunction = [&scriptable]() { scriptable.onDestroy(); };
            onUpdateFunction = [&scriptable](time::Time dt) { scriptable.onUpdate(dt); };
        }
    };

    component(CppScript) {
        serializable()
        const char* cppName = nullptr;
        Scriptable* scriptable = nullptr;

        CppScript() = default;

        CppScript(const char* cppName, Scriptable* scriptable)
        : scriptable(scriptable), cppName(cppName) {}
    };
}
