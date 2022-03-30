#pragma once

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

    struct BitmapData {
        int imageWidth;
        int imageHeight;
        uint8_t* buffer;
        int* bufferSize;
    };

    class BitmapFile final {

    private:
        BitmapFile() = default;
        ~BitmapFile() = default;

    public:
        static bool write(const std::string& bitmapPath, const BitmapData& bitmapData);
    };

}