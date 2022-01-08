//
// Created by mecha on 11.09.2021.
//

#include "FileSystem.h"

#include <direct.h>
#include <sstream>
#include <fstream>

namespace engine {

    std::string FileSystem::getCurrentWorkingDirectory() {
        char buff[FILENAME_MAX]; //create string buffer to hold path
        _getcwd(buff, FILENAME_MAX);
        return buff;
    }

    std::string FileSystem::read(const std::string &filePath) {
        std::string result;
        std::ifstream input(filePath, std::ios::in | std::ios::binary);

        if (input) {
            input.seekg(0, std::ios::end);
            size_t size = input.tellg();

            if (size != -1) {
                result.resize(size);
                input.seekg(0, std::ios::beg);
                input.read(&result[0], size);
            } else {
                ENGINE_ERR("Could not read from file '{0}'", filePath);
            }
        } else {
            ENGINE_ERR("Could not open file '{0}'", filePath);
        }

        return result;
    }

    bool FileSystem::write(const std::string &filePath, const std::string &source) {
        std::ofstream file;
        file.open(filePath);
        file << source;
        file.close();
        return true;
    }

    bool FileSystem::copy(const std::string &src, const std::string &dest) {
        auto isCopied = std::filesystem::copy_file(src, dest);
        if (isCopied) {
            ENGINE_INFO("File has been copied. Source: {0} , Destination: {1}", src, dest);
        } else {
            ENGINE_ERR("File has not been copied. Source: {0} , Destination: {1}", src, dest);
        }
        return isCopied;
    }

    bool FileSystem::remove(const std::string &target) {
        auto isRemoved = std::filesystem::remove(target);
        if (isRemoved) {
            ENGINE_INFO("File has been removed. File path: {0}", target);
        } else {
            ENGINE_ERR("File has not been removed. File path: {0}", target);
        }
        return isRemoved;
    }

    // Returns the folder path of full file path.
    std::string FileSystem::getFolderPath(const std::string &fullPath) {
        size_t folderPathSize = fullPath.find_last_of("/\\");
        return fullPath.substr(0, folderPathSize + 1);
    }

    std::string FileSystem::readWithIncludes(const std::string &fullPath, const std::string &includeToken) {
        std::string result;
        std::ifstream file(fullPath);
        static bool isRecursiveCall = false;

        if (!file.is_open()) {
            ENGINE_ERR("Could not open file: {0}", fullPath);
            return result;
        }

        std::string lineBuffer;
        while (std::getline(file, lineBuffer)) {
            // Find new include token
            if (lineBuffer.find(includeToken) != std::string::npos) {
                // Remove include token
                lineBuffer.erase(0, includeToken.size() + 1);

                // Concat include path and relative path
                std::string includeFullPath = getFolderPath(fullPath);
                lineBuffer.insert(0, includeFullPath);

                // Recursively adding includes to result
                isRecursiveCall = true;
                result += readWithIncludes(lineBuffer, includeToken);

                continue;
            }

            result += lineBuffer + '\n';
        }

        // Only add the null terminator at the end of the complete file
        if (!isRecursiveCall) {
            result += '\0';
        }

        file.close();

        return result;
    }

    std::string FileSystem::getFileName(const std::string &filePath) {
        // Extract file name from filepath
        uint32_t lastSlash = filePath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        uint32_t lastDot = filePath.rfind('.');
        uint32_t count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
        return filePath.substr(lastSlash, count);
    }

    void FileSystem::newFile(const std::string &filePath) {
        std::ofstream newFile(filePath);
        newFile.close();
    }

    std::filesystem::path FileSystem::toPath(const wchar_t *path) {
        return { path };
    }
}