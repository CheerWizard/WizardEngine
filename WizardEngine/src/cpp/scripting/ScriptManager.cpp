#include <scripting/ScriptManager.h>

#include <Windows.h>

namespace engine::scripting {

    void* ScriptManager::s_Library = nullptr;
    vector<Scriptable*> ScriptManager::s_Scripts;

    void ScriptManager::init(const std::string& libPath) {
        s_Library = static_cast<HINSTANCE>(LoadLibrary(libPath.c_str()));
        if (!s_Library) {
            ENGINE_ERR("ScriptManager::load: Failed to load library {0}", libPath);
        }
    }

    Scriptable* ScriptManager::load(const std::string &scriptName) {
        std::string functionName = "new" + scriptName;
        Scriptable* newScript = ((newScriptable) GetProcAddress(static_cast<HINSTANCE>(s_Library), functionName.c_str()))();
        newScript->onCreate();
        s_Scripts.emplace_back(newScript);
        return newScript;
    }

    void ScriptManager::free() {
        for (auto script : s_Scripts) {
            if (script) {
                script->onDestroy();
                delete script;
            }
        }
        s_Scripts.clear();

        if (s_Library) {
            FreeLibrary(static_cast<HINSTANCE>(s_Library));
            s_Library = nullptr;
        }
    }

    void ScriptManager::update(Time dt) {
        for (auto script : s_Scripts) {
            script->onUpdate(dt);
        }
    }
}