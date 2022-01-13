//
// Created by mecha on 08.01.2022.
//

#include "Build.h"
#include "FileSystem.h"

namespace engine {

    Task<void(const std::string&, std::string&), const std::string&, std::string&> LibSO::createTask = {
            "ScriptLibrary_Task",
            "ScriptLibrary_Thread",
            createLib
    };

    Task<void(const std::string&, std::string&), const std::string&, std::string&> Executable::createTask = {
            "CreateExe_Task",
            "CreateExe_Thread",
            createExe
    };

    Task<void(const std::string&), const std::string&> Executable::runTask = {
            "RunExe_Task",
            "RunExe_Thread",
            runExe
    };

    void LibSO::createLib(const std::string &filePath, std::string &libPath) {
        ENGINE_INFO("createLib({0})", filePath);
        auto fileName = FileSystem::getFileName(filePath);
        FileSystem::newDirectory("objects");
        FileSystem::newDirectory("lib");
        auto objectPath = "objects/" + fileName + ".o";
        libPath = "lib/" + fileName + ".so";
        // compile code to Object file, using G++/GCC compiler.
        auto cmdCompile = "g++ -std=c++17 -c -o " + objectPath + " " + filePath;
        ENGINE_INFO(cmdCompile);
        system(cmdCompile.c_str());
        // create dynamic .so library
        auto cmdGenLib = "gcc -shared -o " + libPath + " " + objectPath;
        ENGINE_INFO(cmdGenLib);
        system(cmdGenLib.c_str());
    }

    void LibSO::create(const std::string &filePath, std::string &libPath) {
        createTask.run(filePath, libPath);
    }

    void Executable::create(const std::string &srcPath, std::string &exePath) {
        createTask.run(srcPath, exePath);
    }

    void Executable::createExe(const std::string &srcPath, std::string &exePath) {
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
        runTask.run(path);
    }

}