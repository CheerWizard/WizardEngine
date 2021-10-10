//
// Created by mecha on 11.09.2021.
//

#include "File.h"

#include <direct.h>
#include <sstream>
#include <fstream>

namespace engine {

    std::string File::readAsset(const std::string &assetName) const {
        return read(createPath(assetName));
    }

    void File::createName() {
        // Extract name from filepath
        auto lastSlash = assetPath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = assetPath.rfind('.');
        auto count = lastDot == std::string::npos ? assetPath.size() - lastSlash : lastDot - lastSlash;
        name = assetPath.substr(lastSlash, count);
    }

    std::string File::createPath(const std::string &assetName) const {
        std::stringstream assetFullPath;
        assetFullPath << assetPath << "/" << assetName << getExtensionName();
        return assetFullPath.str();
    }

    std::string File::getCurrentWorkingDirectory() {
        char buff[FILENAME_MAX]; //create string buffer to hold path
        _getcwd(buff, FILENAME_MAX);
        return buff;
    }

    std::string File::read(const std::string &filePath) {
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

    bool File::write(const std::string &filePath, const std::string &source) {
        std::ofstream file;
        file.open(filePath);
        file << source;
        file.close();
        return true;
    }

    bool File::copy(const std::string &src, const std::string &dest) {
        auto isCopied = std::filesystem::copy_file(src, dest);
        if (isCopied) {
            ENGINE_INFO("File has been copied. Source: {0} , Destination: {1}", src, dest);
        } else {
            ENGINE_ERR("File has not been copied. Source: {0} , Destination: {1}", src, dest);
        }
        return isCopied;
    }

    bool File::remove(const std::string &target) {
        auto isRemoved = std::filesystem::remove(target);
        if (isRemoved) {
            ENGINE_INFO("File has been removed. File path: {0}", target);
        } else {
            ENGINE_ERR("File has not been removed. File path: {0}", target);
        }
        return isRemoved;
    }

}