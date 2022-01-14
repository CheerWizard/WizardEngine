//
// Created by mecha on 08.01.2022.
//

#pragma once

#include "Task.h"
#include "../platform/Platform.h"
#include "unordered_map"

namespace engine {

    typedef std::unordered_map<std::string, std::string> StringsMap;
    typedef std::unordered_map<std::string, HMODULE> LoadedLibMap;

    class Library {

    public:
        static void compile(const std::string &srcPath);
        static void generate(const std::string &libName);
        static void clear();
        static void free(const std::string &libName);

        template<typename Object>
        static Object* load(const std::string& libName);

    private:
        static void compileTask(const std::string &srcPath);
        static void generateLib(const std::string& libName);
        static bool exists(const std::string& key);

    private:
        static StringsMap libMap;
        static StringsMap objectsMap;
        static LoadedLibMap loadedLibMap;

    };

    template<typename Object>
    Object* Library::load(const std::string &libName) {
        ENGINE_INFO("load({0})", libName);
        Object* object = nullptr;
        if (!exists(libName)) {
            ENGINE_ERR("Library with name {0} does not exists in cache!");
            return object;
        }
        auto libPath = libMap[libName];
        typedef Object* (__stdcall *FUNCTION)(); // function pointer to exported create_object function.

        HMODULE lib = LOAD_LIB(libPath.c_str());
        if (!lib) {
            ENGINE_ERR("Unable to load library {0}", libPath);
            return object;
        } else {
            loadedLibMap[libName] = lib;
            ENGINE_INFO("Library {0} has been loaded!", libPath);
            FUNCTION createObject = (FUNCTION) GetProcAddress(lib, "create_object");

            if (createObject) {
                ENGINE_INFO("Library function {0} has been loaded!", "create");
                object = createObject();
            } else {
                ENGINE_ERR("Unable to load library function {0}", "create");
            }

            return object;
        }
    }

    class Executable {

    public:
        static void generate(const std::string &srcPath, std::string& exePath);
        static void run(const std::string& path);

    private:
        static void generateExe(const std::string &srcPath, std::string& exePath);
        static void runExe(const std::string& path);

    };

}