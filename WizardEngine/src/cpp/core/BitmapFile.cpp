//
// Created by mecha on 28.03.2022.
//

#include <core/BitmapFile.h>
#include <core/Logger.h>

namespace engine {

    std::unordered_map<char, Character> BitmapFile::chars = {};

    // convert normal buffer to bitmap buffer
    uint8_t* toBitmapBuffer(const int& imageWidth, const int& imageHeight, const uint8_t* buffer, int* bufferSize) {
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

    bool BitmapFile::create(
            const std::string& fontPath,
            const int& fontSize,
            const std::string& bitmapPath,
            const std::string& widthsPath
    ) {
        FT_Library lib;
        FT_Error error;
        FT_Face face;
        FT_UInt glyphIndex;

        // init freetype
        error = FT_Init_FreeType(&lib);
        if (error != FT_Err_Ok) {
            ENGINE_ERR("BitmapFile : failed to load FreeType library : error {0}", error);
            return false;
        }

        // load font
        error = FT_New_Face(lib, fontPath.c_str(), 0, &face);
        if (error == FT_Err_Unknown_File_Format) {
            ENGINE_ERR("BitmapFile : failed to open a file {0} , unknown file format!", fontPath);
            return false;
        } else if (error) {
            ENGINE_ERR("BitmapFile : failed to open a file {0} , error : {1}", fontPath, error);
            return false;
        }

        // set font size
        error = FT_Set_Pixel_Sizes(face, 0, fontSize);
        if (error) {
            ENGINE_ERR("BitmapFile : failed to set a font size : {0}, error : {1}", fontSize, error);
        }

        // create bitmap font
        int imageWidth = (fontSize + 2) * 16;
        int imageHeight = (fontSize + 2) * 8;

        // create a buffer for the bitmap
        auto* buffer = new uint8_t[imageWidth * imageHeight * 4];
        memset(buffer, 0,imageWidth * imageHeight * 4);

        // create an array to save the character widths
        int* widths = new int[128];

        // we need to find the character that goes below the baseline by the biggest value
        int maxUnderBaseline = 0;
        for (int i = 32; i < 127; ++i) {
            // get the glyph index from character code
            glyphIndex = FT_Get_Char_Index(face, i);

            // load the glyph image into the slot
            error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
            if (error) {
                ENGINE_ERR("BitmapFile : failed to load a glyph, error : {0}", error);
                continue;
            }

            // get the glyph metrics
            const FT_Glyph_Metrics& glyphMetrics = face->glyph->metrics;

            // find the character that reaches below the baseline by the biggest value
            int glyphHang = (glyphMetrics.horiBearingY - glyphMetrics.height) / 64;
            if (glyphHang < maxUnderBaseline) {
                maxUnderBaseline = glyphHang;
            }
        }

        // draw all characters
        for (int i = 0; i < 128; ++i) {
            // get the glyph index from character code
            glyphIndex = FT_Get_Char_Index(face, i);

            // load the glyph image into the slot
            error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
            if (error) {
                ENGINE_ERR("BitmapFile : failed to load a glyph, error : {0}", error);
                continue;
            }

            // convert to an anti-aliased bitmap
            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            if (error) {
                ENGINE_ERR("BitmapFile : failed to render a glyph, error : {0}", error);
            }

            // save the character width
            auto w = face->glyph->metrics.width / 64;
            auto h = face->glyph->metrics.height / 64;
            widths[i] = w;
            // find the tile position where we have to draw the character
            int x = (i % 16) * (fontSize + 2);
            int y = (i / 16) * (fontSize + 2);
            x += 1; // 1 pixel padding from the left side of the tile
            y += (fontSize + 2) - face->glyph->bitmap_top + maxUnderBaseline - 1;
            // store metrics data into Character struct
            CharVertex charVertices[4] = {
                    CharVertex { { 0, 0 }, { x, y } },
                    CharVertex { { 0, 0 }, { x + w, y } },
                    CharVertex { { 0, 0 }, { x, y + h } },
                    CharVertex { { 0, 0 }, { x + w, y + h } }
            };
            auto charVertexData = VertexData<CharVertex> { charVertices, 0, 4 };
            auto charVertexDataComponent = VertexDataComponent<CharVertex> {
                charVertexData, true, 0, TRIANGLE_STRIP
            };
            auto character = Character { charVertexDataComponent, { w, h } };
            // map char and appropriate Character struct
            chars.insert(std::pair<char, Character>(i, character));
            ENGINE_INFO("BitmapFile : mapping char '{0}' into Character : ", i);
            for (auto& charVertex : charVertices) {
                ENGINE_INFO("CharVertex[x : {0}, y : {1}]", charVertex.uv.x, charVertex.uv.y);
            }
            ENGINE_INFO("Character[size : {0} , {1}]", character.size.x, character.size.y);
            // draw the character
            const FT_Bitmap& bitmap = face->glyph->bitmap;
            for (int xx = 0; xx < bitmap.width; ++xx) {
                for (int yy = 0; yy < bitmap.rows; ++yy) {
                    uint8_t r = bitmap.buffer[(yy * (bitmap.width) + xx)];
                    int z = (y + yy) * imageWidth * 4 + (x + xx) * 4;
                    buffer[z] = r;
                    buffer[z + 1] = r;
                    buffer[z + 2] = r;
                    buffer[z + 3] = 255;
                }
            }
        }

        // convert the buffer to the bitmap format
        int paddedSize = 0;
        unsigned char* bitmapBuffer = toBitmapBuffer(imageWidth, imageHeight, buffer, &paddedSize);

        BitMapFileHeader bmfh;
        bmfh.type = 0x4d42; // 0x4d42 = 'BM'
        bmfh.filesize = 14 + sizeof(BitMapInfoHeader) + paddedSize;
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
        fwrite(bitmapBuffer, sizeof(unsigned char), paddedSize, file);

        fclose(file);
        delete[] bitmapBuffer;
        delete[] buffer;

        // save the widths file
        std::ofstream ofs;
        ofs.open(widthsPath);
        if (ofs.is_open()) {
            for (int i = 0 ; i < 128 ; ++i) {
                ofs << widths[i] << std::endl;
            }
            ofs.close();
        } else {
            ENGINE_ERR("BitmapFile : failed to save widths file {0}", widthsPath);
            return false;
        }
        delete[] widths;

        // shutdown freetype
        error = FT_Done_FreeType(lib);
        if (error != FT_Err_Ok) {
            ENGINE_ERR("BitmapFile : failed to close FreeType library, error : {0}", error);
            return false;
        }

        return true;
    }

}