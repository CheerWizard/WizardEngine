#pragma once

#include <graphics/core/buffer_data/VertexData.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>

namespace engine {

    struct BitMapFileHeader {
        short type;
        int filesize;
        short reserved1;
        short reserved2;
        int offset;
    };

    struct BitMapInfoHeader {
        int headerSize;
        int width;
        int height;
        short planes;
        short bitsPerPixel;
        int compression;
        int imageSize;
        int xPixelsPerMeter;
        int yPixelsPerMeter;
        int colorsUsed;
        int importantColorsUsed;
    };

    struct CharVertex {
        glm::vec2 position = { 0.5, 0.5 };
        glm::vec2 uv = { 0, 0 };
    };

    struct Character {
        VertexDataComponent<CharVertex> vertexDataComponent;
        glm::vec2 size = { 0, 0 };
    };

    class BitmapFile final {

    private:
        BitmapFile() = default;
        ~BitmapFile() = default;

    public:
        static bool create(
                const std::string& fontPath,
                const int& fontSize,
                const std::string& bitmapPath,
                const std::string& widthsPath
        );

    private:
        static std::unordered_map<char, Character> chars;
    };

}