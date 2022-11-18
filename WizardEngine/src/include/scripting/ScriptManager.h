//
// Created by mecha on 05.10.2022.
//

#pragma once

#include <scripting/Scriptable.h>

namespace engine::scripting {

    class ENGINE_API ScriptManager final {

    public:
        static void init(const std::string& libPath = path);
        static Scriptable* load(const std::string& scriptName);
        static void free();

    private:
        static void* library;
        static std::string path;
    };

}
