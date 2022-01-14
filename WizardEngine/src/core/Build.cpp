//
// Created by mecha on 08.01.2022.
//

#include "Build.h"
#include "FileSystem.h"
#include "sstream"

namespace engine {

    StringsMap Library::libMap = {};
    StringsMap Library::objectsMap = {};
    LoadedLibMap Library::loadedLibMap = {};

    void Library::generateLib(const std::string &libName) {
        ENGINE_INFO("generateLib({0})", libName);
        FileSystem::newDirectory("lib");
        auto libPath = "lib/" + libName + ".so";
        // create dynamic .so library
        std::stringstream cmd;
        cmd << "g++ -std=c++17 -shared -o " << libPath;
        for (const auto &[objName, objPath] : objectsMap) {
            cmd << " " << objPath;
        }
        auto cmdStr = cmd.str();
        ENGINE_INFO(cmdStr);
        system(cmdStr.c_str());
        libMap[libName] = libPath;
    }

    void Library::generate(const std::string &libName) {
        Task<void(const std::string&), const std::string&> task = {
                "GenerateLib_Task",
                "GenerateLib_Thread",
                generateLib
        };
        task.run(libName);
    }

    bool Library::exists(const std::string& key) {
        return libMap.find(key) != libMap.end();
    }

    void Library::clear() {
        ENGINE_INFO("Library: clear()");
        libMap.clear();
        loadedLibMap.clear();
        objectsMap.clear();
    }

    void Library::free(const std::string &libName) {
        ENGINE_INFO("free({0})", libName);
        if (!exists(libName)) {
            ENGINE_ERR("Can't free library {0} as it was not loaded!");
        } else {
            FREE_LIB(loadedLibMap[libName]);
        }
    }

    void Library::compile(const std::string &srcPath) {
        Task<void(const std::string&), const std::string&> task = {
                "Compile_Task",
                "Compile_Thread",
                compileTask
        };
        task.run(srcPath);
    }

    void Library::compileTask(const std::string &srcPath) {
        ENGINE_INFO("compileTask({0})", srcPath);
        auto objectName = FileSystem::getFileName(srcPath);
        FileSystem::newDirectory("objects");
        auto objectPath = "objects/" + objectName + ".o";
        // compile code to Object file, using G++/GCC compiler.
        auto cmdCompile = "g++ -std=c++17 -c -o " + objectPath + " " + srcPath;
        ENGINE_INFO(cmdCompile);
        system(cmdCompile.c_str());
        objectsMap[objectName] = objectPath;
    }

    void Executable::generate(const std::string &srcPath, std::string &exePath) {
        Task<void(const std::string&, std::string&), const std::string&, std::string&> task = {
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

    void Executable::runExe(const std::string &path) {
        ENGINE_INFO("runExe({0})", path);
        auto cmd = ".\\" + path;
        ENGINE_INFO(cmd);
        system(cmd.c_str());
    }

    void Executable::run(const std::string &path) {
        Task<void(const std::string&), const std::string&> task = {
                "RunExe_Task",
                "RunExe_Thread",
                runExe
        };
        task.run(path);
    }

}