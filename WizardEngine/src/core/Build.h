//
// Created by mecha on 08.01.2022.
//

#pragma once

#include "Task.h"

namespace engine {

    class LibSO {

    public:
        static void create(const std::string &filePath, std::string &libPath);

    private:
        static void createLib(const std::string &filePath, std::string& libPath);

    private:
        static Task<void(const std::string&, std::string&), const std::string&, std::string&> createTask;

    };

    class Executable {

    public:
        static void create(const std::string &srcPath, std::string& exePath);
        static void run(const std::string& path);

    private:
        static void createExe(const std::string &srcPath, std::string& exePath);
        static void runExe(const std::string& path);

    private:
        static Task<void(const std::string&, std::string&), const std::string&, std::string&> createTask;
        static Task<void(const std::string&), const std::string&> runTask;

    };

}