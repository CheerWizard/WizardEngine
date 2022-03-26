//
// Created by mecha on 10.11.2021.
//

#pragma once

#include <core/FileSystem.h>

#define RUNTIME_TEXTURES_PATH "assets/textures"

namespace engine {

    struct TextureData {
        int width = 0, height = 0, channels = 0;
        void* data = nullptr;
    };

    class TextureFile final {

    private:
        TextureFile() = default;
        ~TextureFile() = default;

    public:
        // read functions will return NULL value, if the read process failed!
        static TextureData read(const std::string_view &fileName);
        static TextureData read(const std::string_view &fileName, const std::string_view &texturesPath);

        static void free(void* data);
    };

}
