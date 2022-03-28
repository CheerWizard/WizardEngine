//
// Created by mecha on 27.03.2022.
//

#pragma once

#include <cstdint>
#include <string>

namespace engine {

    struct BitMapFileHeader {
        short type;
        int filesize;
        short reserved1;
        short reserved2;
        int offset;
    };

    struct BitMapInfoHeader {
        int infoHeaderSize;
        int width;
        int height;
        short planes;
        short bitsPerPixel;
        int compression;
        int imageSize;
        int xPixelsPerMeter;
        int yPixelsPerMeter;
        int colorUsed;
        int importantColorUsed;
    };

    struct BitmapData {
        int imageWidth;
        int imageHeight;
        uint8_t* buffer;
        int* bufferSize;
    };

    class Bitmap final {

    private:
        Bitmap() = default;

    public:
        static uint8_t* toBitmapBuffer(const BitmapData& bitMapData);
        static bool write(const std::string& bitmapPath, const BitmapData& bitMapData);

    };

}