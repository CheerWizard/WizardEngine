//
// Created by mecha on 08.01.2022.
//

#include <core/Build.h>
#include <core/filesystem.h>
#include "sstream"

namespace engine {

    StringsMap Libs::map = {};
    StringsMap Objects::map = {};
    HModuleMap HModules::map = {};

    void Libs::generate(const std::string &libName) {
        VoidTask<const std::string&> task = {
                "GenerateLib_Task",
                "GenerateLib_Thread",
                generateLib
        };
        task.run(libName);
    }

    void Libs::generateLib(const std::string &libName) {
        ENGINE_INFO("generateLib({0})", libName);
        FileSystem::newDirectory("lib");
        auto libPath = "lib/" + libName + ".so";
        // create dynamic .so library
        std::stringstream cmd;
        cmd << "g++ -std=c++17 -shared -o " << libPath;
        for (const auto &[objName, objPath] : Objects::getAll()) {
            cmd << " " << objPath;
        }
        auto cmdStr = cmd.str();
        ENGINE_INFO(cmdStr);
        system(cmdStr.c_str());
        add(libName, libPath);
    }

    bool Libs::exists(const std::string& key) {
        return map.find(key) != map.end();
    }

    void Libs::clear() {
        ENGINE_INFO("Library: clear()");
        map.clear();
    }

    void Objects::compile(const std::string &srcPath) {
        VoidTask<const std::string&> task = {
                "Compile_Task",
                "Compile_Thread",
                compileTask
        };
        task.run(srcPath);
    }

    void Objects::compileTask(const std::string &srcPath) {
        ENGINE_INFO("compileTask({0})", srcPath);
        auto objectName = FileSystem::getFileName(srcPath);
        FileSystem::newDirectory("objects");
        auto objectPath = "objects/" + objectName + ".o";
        // compile code to Object file, using G++/GCC compiler.
        auto cmdCompile = "g++ -std=c++17 -c -o " + objectPath + " " + srcPath;
        ENGINE_INFO(cmdCompile);
        system(cmdCompile.c_str());
        add(objectName, objectPath);
    }

    void Libs::add(const std::string &name, const std::string& path) {
        ENGINE_INFO("add(libName: {0}, libPath: {1})", name, path);
        map[name] = path;
    }

    void Libs::remove(const std::string &name) {
        ENGINE_INFO("remove(libName: {0})", name);
        map.erase(name);
    }

    const std::string& Libs::get(const std::string &name) {
        return map[name];
    }

    void Objects::add(const std::string &name, const std::string &path) {
        ENGINE_INFO("add(objectName: {0}, objectPath: {1})", name, path);
        map[name] = path;
    }

    void Objects::remove(const std::string &name) {
        ENGINE_INFO("remove(objectName: {0})", name);
        map.erase(name);
    }

    const std::string &Objects::get(const std::string &name) {
        return map[name];
    }

    const StringsMap &Objects::getAll() {
        return map;
    }

    void Objects::clear() {
        map.clear();
    }

    void HModules::add(const std::string &name, HMODULE const &hmodule) {
        ENGINE_INFO("add(hmoduleName: {0})", name);
        map[name] = hmodule;
    }

    void HModules::remove(const std::string &name) {
        map.erase(name);
    }

    const HMODULE &HModules::get(const std::string& name) {
        return map[name];
    }

    void HModules::free(const std::string &name) {
        ENGINE_INFO("free({0})", name);
        if (!exists(name)) {
            ENGINE_ERR("Can't free library {0} as it was not loaded!");
        } else {
            FREE_LIB(get(name));
        }
    }

    bool HModules::exists(const std::string &name) {
        return map.find(name) != map.end();
    }

    void HModules::clear() {
        map.clear();
    }

    void Executable::generate(const std::string &srcPath, std::string &exePath) {
        VoidTask<const std::string&, std::string&> task = {
              "GenerateExe_Task",
              "GenerateExe_Thread",
              generateExe
        };
        task.run(srcPath, exePath);
    }

    void Executable::generateExe(const std::string &srcPath, std::string &exePath) {
        ENGINE_INFO("createExe({0})", srcPath);

        auto cmdCreate = "g++ -Llib/ -Wall -o code " + srcPath + " -llibrary";
        ENGINE_INFO(cmdCreate);
        system(cmdCreate.c_str());

        auto cmdSetLibPath = "export LD_LIBRARY_PATH=lib/:$LD_LIBRARY_PATH";
        ENGINE_INFO(cmdSetLibPath);
        system(cmdSetLibPath);
    }

    void Executable::run(const std::string &path) {
        VoidTask<const std::string&> task = {
                "RunExe_Task",
                "RunExe_Thread",
                runExe
        };
        task.run(path);
    }

    void Executable::runExe(const std::string &path) {
        ENGINE_INFO("runExe({0})", path);
        auto cmd = ".\\" + path;
        ENGINE_INFO(cmd);
        system(cmd.c_str());
    }

}