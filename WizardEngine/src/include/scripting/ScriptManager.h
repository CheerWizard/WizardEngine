#pragma once

#include <scripting/Scriptable.h>

namespace engine::scripting {

    class ENGINE_API ScriptManager final {

    public:
        static void init(const std::string& libPath);
        static Scriptable* load(const std::string& scriptName);
        static void update(Time dt);
        static void free();

    private:
        static void* s_Library;
        static vector<Scriptable*> s_Scripts;
    };

}
