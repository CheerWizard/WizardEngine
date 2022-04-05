//
// Created by mecha on 11.09.2021.
//

#pragma once

#include <core/String.h>
#include <io/Logger.h>

#include <filesystem>

#define CURRENT_WORKING_DIR engine::filesystem::getCurrentWorkingDirectory()
#define RUNTIME_ASSET_PATH "assets"

namespace engine::filesystem {

    typedef std::filesystem::path fpath;
    typedef std::filesystem::file_status fstatus;
    typedef std::error_code error;

    bool write(const fpath &filePath, const std::string &source);
    bool write(const fpath &filePath, const int* data, const uint32_t& dataSize);
    std::string getCurrentWorkingDirectory();
    bool copy(const std::string &src, const std::string &dest);
    bool remove(const std::string &target);
    bool replace(const std::string &src, const std::string &dest);
    std::string getFolderPath(const std::string &fullPath);
    std::string read(const std::string &filePath);
    std::string readWithIncludes(const std::string &fullPath, const std::string &includeToken);
    std::string getFileName(const std::string &filePath);
    void newFile(const fpath &filePath);
    std::filesystem::path toPath(const wchar_t* path);
    error move(const fpath& oldPath, const fpath& newPath);
    error rename(const fpath& filePath, const std::string& newFileName);
    void newFile(const fpath& currentDir, const std::string& newFileName);
    void newDirectory(const fpath& path);
    bool exists(const fpath& path);

}