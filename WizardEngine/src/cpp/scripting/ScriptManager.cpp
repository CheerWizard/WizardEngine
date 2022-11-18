//
// Created by mecha on 05.10.2022.
//

#include <scripting/ScriptManager.h>

#include <Windows.h>

namespace engine::scripting {

    void* ScriptManager::library = nullptr;
    std::string ScriptManager::path = "ScriptEngine.dll";

    void ScriptManager::init(const std::string& libPath) {
        // load library
        library = static_cast<HINSTANCE>(LoadLibrary(libPath.c_str()));
        if (!library) {
            ENGINE_ERR("ScriptManager::load: Failed to load library {0}", libPath);
        }
    }

    Scriptable* ScriptManager::load(const std::string &scriptName) {
        // load object
        std::string functionName = "new" + scriptName;
        return ((newScriptable) GetProcAddress(static_cast<HINSTANCE>(library), functionName.c_str()))();
    }

    void ScriptManager::free() {
        if (library) {
            FreeLibrary(static_cast<HINSTANCE>(library));
            library = nullptr;
        }
    }
}