#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <core/core.h>

namespace engine::io {

    struct ENGINE_API BitMapFileHeader {
        short type;
        int filesize;
        short reserved1;
        short reserved2;
        int offset;
    };

    struct ENGINE_API BitMapInfoHeader {
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

    struct ENGINE_API BitmapData {
        int imageWidth;
        int imageHeight;
        uint8_t* buffer;
        int* bufferSize;
    };

    class ENGINE_API BitmapFile final {

    private:
        BitmapFile() = default;
        ~BitmapFile() = default;

    public:
        static bool write(const std::string& bitmapPath, const BitmapData& bitmapData);
    };

}