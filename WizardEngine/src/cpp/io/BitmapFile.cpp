//
// Created by mecha on 28.03.2022.
//

#include <io/BitmapFile.h>
#include <io/Logger.h>

namespace engine::io {

    // convert normal buffer to bitmap buffer
    uint8_t* toBitmapBuffer(const BitmapData& bitmapData) {
        const int& imageWidth = bitmapData.imageWidth;
        const int& imageHeight = bitmapData.imageHeight;
        const uint8_t* buffer = bitmapData.buffer;
        int* bufferSize = bitmapData.bufferSize;
        int rowSize = ((24 * imageWidth + 31) / 32) * 4;
        int padding = 0;
        while ((imageWidth * 3 + padding) % 4 != 0) padding++;

        *bufferSize = rowSize * imageHeight;
        auto* bitmapBuffer = new uint8_t[*bufferSize];
        memset(bitmapBuffer , 0, *bufferSize);

        for (int y = 0; y < imageHeight; ++y ) {
            for (int x = 0; x < imageWidth; ++x) {
                // position in buffer
                int pos = y * imageWidth * 4 + x * 4;
                // position in padded bitmap buffer
                int newPos = (imageHeight - y - 1) * (imageWidth * 3 + padding) + x * 3;
                bitmapBuffer[newPos + 0] = buffer[pos + 2]; // swap r and b
                bitmapBuffer[newPos + 1] = buffer[pos + 1]; // g stays
                bitmapBuffer[newPos + 2] = buffer[pos + 0]; // swap b and r
            }
        }

        return bitmapBuffer;
    }

    bool BitmapFile::write(const std::string& bitmapPath, const BitmapData& bitmapData) {
        unsigned char* bitmapBuffer = toBitmapBuffer(bitmapData);
        int bufferSize = *bitmapData.bufferSize;
        int imageWidth = bitmapData.imageWidth;
        int imageHeight = bitmapData.imageHeight;

        BitMapFileHeader bmfh;
        bmfh.type = 0x4d42; // 0x4d42 = 'BM'
        bmfh.filesize = 14 + sizeof(BitMapInfoHeader) + bufferSize;
        bmfh.reserved1 = 0; // not used
        bmfh.reserved2 = 0; // not used
        bmfh.offset = 54;

        BitMapInfoHeader bmih;
        bmih.headerSize = sizeof(BitMapInfoHeader);
        bmih.width = imageWidth;
        bmih.height = imageHeight;
        bmih.planes = 1;
        bmih.bitsPerPixel = 24;
        bmih.compression = 0;
        bmih.imageSize = 0;
        bmih.xPixelsPerMeter = 0;
        bmih.yPixelsPerMeter = 0;
        bmih.colorsUsed = 0;
        bmih.importantColorsUsed = 0;

        FILE* file;
        if ((file = fopen(bitmapPath.c_str() , "wb")) == nullptr) {
            ENGINE_ERR("BitmapFile : failed to save bitmap file {0}", bitmapPath);
            return false;
        }

        // write file header
        fwrite(&bmfh.type, sizeof(short), 1, file);
        fwrite(&bmfh.filesize, sizeof(int), 1, file);
        fwrite(&bmfh.reserved1, sizeof(short), 1, file);
        fwrite(&bmfh.reserved2, sizeof(short), 1, file);
        fwrite(&bmfh.offset, sizeof(int), 1, file);

        // write info header
        fwrite(&bmih, sizeof(BitMapInfoHeader), 1, file);

        // write data
        fwrite(bitmapBuffer, sizeof(unsigned char), bufferSize, file);

        fclose(file);
        delete[] bitmapBuffer;

        return true;
    }

}