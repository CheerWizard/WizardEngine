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
    typedef std::filesystem::copy_options copy_options;

    ENGINE_API bool write(const fpath &filePath, const std::string &source);
    ENGINE_API bool write(const fpath &filePath, const int* data, const uint32_t& dataSize);
    ENGINE_API std::string getCurrentWorkingDirectory();
    ENGINE_API bool copy(const std::string &src, const std::string &dest);
    ENGINE_API bool copyRecursive(const char* srcPath, const char* destPath);
    ENGINE_API bool remove(const std::string &target);
    ENGINE_API bool replace(const std::string &src, const std::string &dest);
    ENGINE_API std::string getFolderPath(const std::string &fullPath);
    ENGINE_API std::string read(const std::string &filePath);
    ENGINE_API std::string readWithIncludes(const std::string &fullPath, const std::string &includeToken);
    ENGINE_API std::string getFileName(const std::string &filePath);
    ENGINE_API void newFile(const fpath &filePath);
    ENGINE_API std::filesystem::path toPath(const wchar_t* path);
    ENGINE_API error move(const fpath& oldPath, const fpath& newPath);
    ENGINE_API error rename(const fpath& filePath, const std::string& newFileName);
    ENGINE_API void newFile(const fpath& currentDir, const std::string& newFileName);
    ENGINE_API void newDirectory(const fpath& path);
    ENGINE_API bool exists(const fpath& path);
    ENGINE_API bool write(const char* filepath, const char* data);

}