//
// Created by mecha on 11.09.2021.
//

#include <core/filesystem.h>

#include <direct.h>
#include <sstream>
#include <fstream>

namespace engine::filesystem {

    std::string getCurrentWorkingDirectory() {
        char buff[FILENAME_MAX]; //create string buffer to hold path
        _getcwd(buff, FILENAME_MAX);
        return buff;
    }

    std::string read(const std::string &filePath) {
        ENGINE_INFO("filesystem: reading file from {0}", filePath);
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
                ENGINE_ERR("filesystem: could not read from file '{0}'", filePath);
            }
        } else {
            ENGINE_ERR("filesystem: could not open file '{0}'", filePath);
        }

        return result;
    }

    bool write(const fpath &filePath, const std::string &source) {
        std::ofstream file;
        file.open(filePath);
        file << source;
        file.close();
        return true;
    }

    bool copy(const std::string &src, const std::string &dest) {
        auto isCopied = std::filesystem::copy_file(src, dest);
        if (isCopied) {
            ENGINE_INFO("filesystem: file has been copied. Source: {0} , Destination: {1}", src, dest);
        } else {
            ENGINE_ERR("filesystem: file has not been copied. Source: {0} , Destination: {1}", src, dest);
        }
        return isCopied;
    }

    bool remove(const std::string &target) {
        auto isRemoved = std::filesystem::remove(target);
        if (isRemoved) {
            ENGINE_INFO("filesystem: file has been removed. File path: {0}", target);
        } else {
            ENGINE_ERR("filesystem: file has not been removed. File path: {0}", target);
        }
        return isRemoved;
    }

    // Returns the folder path of full file path.
    std::string getFolderPath(const std::string &fullPath) {
        size_t folderPathSize = fullPath.find_last_of("/\\");
        return fullPath.substr(0, folderPathSize + 1);
    }

    std::string readWithIncludes(const std::string &fullPath, const std::string &includeToken) {
        std::string result;
        std::ifstream file(fullPath);
        static bool isRecursiveCall = false;

        if (!file.is_open()) {
            ENGINE_ERR("filesystem: could not open file: {0}", fullPath);
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

    std::string getFileName(const std::string &filePath) {
        // Extract file name from filepath
        uint32_t lastSlash = filePath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        uint32_t lastDot = filePath.rfind('.');
        uint32_t count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
        return filePath.substr(lastSlash, count);
    }

    void newFile(const fpath &filePath) {
        write(filePath, " ");
    }

    std::filesystem::path toPath(const wchar_t *path) {
        return { path };
    }

    error move(const fpath &oldPath, const fpath &newPath) {
        ENGINE_INFO("filesystem: move({0}, {1})", oldPath, newPath);
        error e;
        std::filesystem::rename(oldPath, newPath, e);
        return e;
    }

    error rename(const fpath &filePath, const std::string &newFileName) {
        ENGINE_INFO("filesystem: rename({0}, {1})", filePath, newFileName);
        auto newPathStr = filePath.string();
        newPathStr = newPathStr.substr(0, newPathStr.find_last_of("\\/"));
        fpath newPath(newPathStr);
        newPath /= newFileName;
        return move(filePath, newPath);
    }

    void newFile(const fpath &currentDir, const std::string &newFileName) {
        newFile(currentDir/newFileName);
    }

    void newDirectory(const fpath &path) {
        if (!std::filesystem::is_directory(path) && !std::filesystem::exists(path)) {
            std::filesystem::create_directory(path);
        }
    }

    bool exists(const fpath &path) {
        return std::filesystem::exists(path);
    }

    bool replace(const std::string& src, const std::string& dest) {
        bool removed = false;
        if (exists(dest)) {
            removed = remove(dest);
        }

        if (removed) {
            return copy(src, dest);
        }

        return removed;
    }

    bool write(const fpath &filePath, const int* data, const uint32_t& dataSize) {
        std::ofstream ofs;
        ofs.open(filePath);

        if (ofs.is_open()) {
            for (int i = 0; i < dataSize; ++i) {
                ofs << data[i] << std::endl;
            }
            ofs.close();
        } else {
            ENGINE_ERR("filesystem : failed write to {0} file with data size {1}", filePath.string(), dataSize);
            return false;
        }

        return true;
    }

}