//
// Created by mecha on 11.09.2021.
//

#include "File.h"

namespace engine {

    std::string File::read() {
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

}