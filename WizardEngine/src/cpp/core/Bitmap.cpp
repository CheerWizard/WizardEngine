//
// Created by mecha on 27.03.2022.
//

#include <core/Bitmap.h>
#include <core/Logger.h>

#include <cstdio>
#include <fstream>

namespace engine {

    uint8_t* Bitmap::toBitmapBuffer(const BitmapData& bitMapData) {
        int imageWidth = bitMapData.imageWidth;
        int imageHeight = bitMapData.imageHeight;
        int* bufferSize = bitMapData.bufferSize;
        uint8_t* buffer = bitMapData.buffer;
        int rowSize = ((24 * imageWidth + 31) / 32) * 4;
        int padding = 0;

        while ((imageWidth*3 + padding) % 4 != 0) padding++;

        *bufferSize = rowSize * imageHeight;
        auto* bitmapBuffer = new uint8_t [*bufferSize];
        memset(bitmapBuffer, 0, *bufferSize);

        for (int y = 0 ; y < imageHeight ; ++y) {
            for (int x = 0 ; x < imageWidth ; ++x) {
                // position in buffer
                int pos = y*imageWidth*4 + x*4;
                // position in padded bitmap buffer
                int newPos = (imageHeight - y - 1 ) * (imageWidth * 3 + padding ) + x * 3;
                bitmapBuffer[newPos + 0] = buffer[pos + 2]; // swap r and b
                bitmapBuffer[newPos + 1] = buffer[pos + 1]; // g stays
                bitmapBuffer[newPos + 2] = buffer[pos + 0]; // swap b and r
            }
        }

        return bitmapBuffer;
    }

    bool Bitmap::write(const std::string& bitmapFileName, const BitmapData& bitMapData) {
        uint8_t* buffer = bitMapData.buffer;
        int bufferSize = *bitMapData.bufferSize;
        int imageWidth = bitMapData.imageWidth;
        int imageHeight = bitMapData.imageHeight;
        uint8_t* bitmapBuffer = toBitmapBuffer(bitMapData);

        BitMapFileHeader bitMapFileHeader = {
                0x4d42, // 0x4d42 = 'BM'
                static_cast<int>(14 + sizeof(BitMapInfoHeader) + bufferSize),
                0, // not used
                0, // not used
                54
        };

        BitMapInfoHeader bitMapInfoHeader = {
                sizeof(BitMapInfoHeader),
                imageWidth,
                imageHeight,
                1,
                24,
                0,
                0,
                0,
                0,
                0,
                0
        };

        FILE* file;
        fopen_s(&file, bitmapFileName.c_str(), "wb");
        if (!file) {
            ENGINE_ERR("Bitmap : failed to save bitmap file {0}", bitmapFileName);
            return false;
        }

        // write file header
        fwrite(&bitMapFileHeader.type, sizeof(short), 1, file);
        fwrite(&bitMapFileHeader.filesize, sizeof(int), 1, file);
        fwrite(&bitMapFileHeader.reserved1, sizeof(short), 1, file);
        fwrite(&bitMapFileHeader.reserved2, sizeof(short), 1, file);
        fwrite(&bitMapFileHeader.offset, sizeof(int), 1, file);
        // write info header
        fwrite(&bitMapInfoHeader, sizeof(BitMapInfoHeader), 1, file);
        // write data
        fwrite(bitmapBuffer, sizeof(uint8_t), bufferSize, file);

        fclose(file);
        delete[] bitmapBuffer;
        delete[] buffer;

        return true;
    }

}