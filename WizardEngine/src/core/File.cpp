//
// Created by mecha on 11.09.2021.
//

#include "File.h"

namespace engine {

    std::string File::read() const {
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

    void File::createName() {
        // Extract name from filepath
        auto lastSlash = filePath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filePath.rfind('.');
        auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
        name = filePath.substr(lastSlash, count);
    }

}