//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "String.h"
#include "Logger.h"

#include <filesystem>

#define CURRENT_WORKING_DIR engine::FileSystem::getCurrentWorkingDirectory()
#define RUNTIME_ASSET_PATH "assets"

namespace engine {

    typedef std::filesystem::path fpath;
    typedef std::filesystem::file_status fstatus;
    typedef std::error_code error;

    class FileSystem final {

    private:
        FileSystem() = default;
        ~FileSystem() = default;

    public:
        static bool write(const fpath &filePath, const std::string &source);
        static std::string getCurrentWorkingDirectory();
        static bool copy(const std::string &src, const std::string &dest);
        static bool remove(const std::string &target);
        static std::string getFolderPath(const std::string &fullPath);
        static std::string read(const std::string &filePath);
        static std::string readWithIncludes(const std::string &fullPath, const std::string &includeToken);
        static std::string getFileName(const std::string &filePath);
        static void newFile(const fpath &filePath);
        static std::filesystem::path toPath(const wchar_t* path);
        static error move(const fpath& oldPath, const fpath& newPath);
        static error rename(const fpath& filePath, const std::string& newFileName);
        static void newFile(const fpath& currentDir, const std::string& newFileName);
        static void newDirectory(const fpath& path);
    };

}