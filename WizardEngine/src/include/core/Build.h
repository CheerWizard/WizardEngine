//
// Created by mecha on 08.01.2022.
//

#pragma once

#include "Task.h"
#include <platform/tools/LibLoader.h>
#include "unordered_map"

namespace engine {

    typedef std::unordered_map<std::string, std::string> StringsMap;
    typedef std::unordered_map<std::string, HMODULE> HModuleMap;

    class Libs final {

    public:
        static void generate(const std::string &name);
        static void clear();
        static void add(const std::string& name, const std::string& libPath);
        static void remove(const std::string& name);
        static bool exists(const std::string& name);
        static const std::string& get(const std::string& name);

    private:
        static void generateLib(const std::string& libName);

    private:
        static StringsMap map;

    };

    class Objects final {

    public:
        static void compile(const std::string &srcPath);
        static void add(const std::string &name, const std::string &path);
        static void remove(const std::string &name);
        static const std::string& get(const std::string& name);
        static const StringsMap& getAll();
        static void clear();

    private:
        static void compileTask(const std::string &srcPath);

    private:
        static StringsMap map;
    };

    class HModules final {

    public:
        static void add(const std::string &name, const HMODULE& hmodule);
        static void remove(const std::string &name);
        static const HMODULE& get(const std::string& name);
        static void free(const std::string &name);
        static void clear();
        static bool exists(const std::string& name);

    private:
        static HModuleMap map;
    };

    class Executable final {

    public:
        static void generate(const std::string &srcPath, std::string& exePath);
        static void run(const std::string& path);

    private:
        static void generateExe(const std::string &srcPath, std::string& exePath);
        static void runExe(const std::string& path);

    };

    template<typename Object>
    Object* createObject(const std::string &libName, const std::string &createFnName) {
        ENGINE_INFO("createObject({0}, {1})", libName, createFnName);
        Object* object = nullptr;
        if (!Libs::exists(libName)) {
            ENGINE_ERR("Library with name {0} does not exists in cache!");
            return object;
        }
        auto libPath = Libs::get(libName);
        typedef void* (*Function)(); // function pointer to factory function

        HMODULE hmodule = LOAD_LIB(libPath.c_str());
        DWORD dwError = 0;
        if (!hmodule) {
            dwError = GetLastError();
            ENGINE_ERR("Unable to load library {0}", libPath);
            ENGINE_ERR("Error : {0}", dwError);
            return object;
        } else {
            HModules::add(libName, hmodule);
            ENGINE_INFO("Library {0} has been loaded!", libPath);
            Function createFn = (Function) GetProcAddress(hmodule, createFnName.c_str());

            if (createFn) {
                ENGINE_INFO("Library function {0} has been loaded!", createFnName);
                object = (Object*)createFn();
            } else {
                ENGINE_ERR("Unable to load library function {0}", createFnName);
            }

//            HModules::free(libName);

            return object;
        }
    }

    template<typename... Args>
    void createFunction(const std::string &libName, const std::string &fnName, Args&&... args) {
        ENGINE_INFO("createFunction({0}, {1})", libName, fnName);
        if (!Libs::exists(libName)) {
            ENGINE_ERR("Library with name {0} does not exists in cache!");
            return;
        }
        auto libPath = Libs::get(libName);
        typedef void (*Function)(Args...); // function pointer to factory function

        HMODULE hmodule = LOAD_LIB(libPath.c_str());
        DWORD dwError = 0;
        if (!hmodule) {
            dwError = GetLastError();
            ENGINE_ERR("Unable to load library {0}", libPath);
            ENGINE_ERR("Error : {0}", dwError);
            return;
        } else {
            HModules::add(libName, hmodule);
            ENGINE_INFO("Library {0} has been loaded!", libPath);
            Function function = (Function) GetProcAddress(hmodule, fnName.c_str());

            if (function) {
                ENGINE_INFO("Library function {0} has been loaded!", fnName);
                function(std::forward<Args>(args)...);
            } else {
                ENGINE_ERR("Unable to load library function {0}", fnName);
            }
//            HModules::free(libName);
        }
    }

}