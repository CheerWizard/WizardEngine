//
// Created by mecha on 11.09.2021.
//

#include "File.h"

#include <direct.h>
#include <sstream>

namespace engine {

    std::string File::read() const {
        return read(path);
    }

    void File::createName() {
        // Extract name from filepath
        auto lastSlash = path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = path.rfind('.');
        auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        name = path.substr(lastSlash, count);
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

    void File::setAssetName(const std::string &assetName) {
        std::stringstream assetFullPath;
        assetFullPath << assetPath << "/" << getAssetPath() << "/" << assetName << getExtensionName();
        path = assetFullPath.str();
        name = assetName;
    }

}