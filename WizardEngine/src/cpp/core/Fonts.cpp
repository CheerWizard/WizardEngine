//
// Created by mecha on 28.03.2022.
//

#include <core/Fonts.h>
#include <core/Logger.h>
#include <core/Bitmap.h>
#include <core/FileSystem.h>

namespace engine {

    void Fonts::init() {
        if (FT_Init_FreeType(&ftLibrary)) {
            ENGINE_CRIT("FreeType : library init() failed!");
        } else {
            ENGINE_INFO("FreeType : library init() succeeded!");
        }
    }

    bool Fonts::load(const std::string &fontPath, const size_t& fontSize) {
        if (exists(fontPath)) {
            fonts[fontPath].setFontSize(fontSize);
            return true;
        }

        FT_Face ftFace;
        bool isLoaded = FT_New_Face(ftLibrary, fontPath.c_str(), 0, &ftFace) == 0;

        if (isLoaded) {
            ENGINE_INFO("FreeType : load font {0} succeeded!", fontPath);
            fonts.insert(std::pair<std::string, FontFace>(
                    fontPath,
                    FontFace(ftFace, fontSize)
            ));
        } else {
            ENGINE_ERR("FreeType : failed to load font {0}", fontPath);
        }

        return isLoaded;
    }

    bool FontFace::initGlyph(const uint8_t &glyph, const FT_Int32& flag) {
        bool isLoaded = FT_Load_Char(face, glyph, flag) == 0;

        if (isLoaded) {
            ENGINE_INFO("FreeType : load glyph {0} succeeded!", glyph);
        } else {
            ENGINE_ERR("FreeType : failed to load glyph {0}", glyph);
        }

        return isLoaded;
    }

    void FontFace::initGlyphs(const size_t& fontSize) {
        setFontSize(fontSize);
        for (uint8_t ch = 0; ch < 128; ch++) {
            if (!initGlyph(ch, FT_LOAD_RENDER)) {
                continue;
            }
            auto fontGlyph = FontGlyph {
                    { face->glyph->bitmap.width, face->glyph->bitmap.rows },
                    { face->glyph->bitmap_left, face->glyph->bitmap_top },
                    face->glyph->advance.x
            };
            glyphs.insert(std::pair<char, FontGlyph>(ch, fontGlyph));
        }
    }

    bool FontFace::setFontSize(const size_t &fontSize) {
        bool isLoaded = FT_Set_Pixel_Sizes(face, 0, fontSize) == 0;

        if (isLoaded) {
            ENGINE_INFO("FreeType : setFontSize(fontSize : {0}) has been succeeded!", fontSize);
        } else {
            ENGINE_ERR("FreeType : setFontSize(fontSize : {0}) has been failed!", fontSize);
        }

        return isLoaded;
    }

    void FontFace::destroy() {
        FT_Done_Face(face);
        glyphs.clear();
    }

    const FT_Face &FontFace::get() {
        return face;
    }

    bool FontFace::renderGlyph(const FT_Render_Mode_& renderMode) {
        bool isRendered = FT_Render_Glyph(face->glyph, renderMode) == 0;

        if (isRendered) {
            ENGINE_INFO("FreeType : Glyph has been rendered!");
        } else {
            ENGINE_ERR("FreeType : Glyph failed to render!");
        }

        return isRendered;
    }

    void Fonts::clear() {
        fonts.clear();
        FT_Done_FreeType(ftLibrary);
    }

    void Fonts::generateBitmap(
            const std::string &fontFilename,
            const int &fontSize,
            const std::string &bitmapFilename,
            const std::string &widthsFilename
    ) {
        bool isLoaded = load(fontFilename, fontSize);
        if (!isLoaded) {
            ENGINE_ERR("FontSource : can't load font to generate bitmap; fontFile = {0}, fontSize = {1}",
                       fontFilename, fontSize);
            return;
        }
        auto fontFace = fonts[fontFilename];
        // create bitmap font
        int imageWidth = (fontSize+2)*16;
        int imageHeight = (fontSize+2)*8;
        // create a buffer for the bitmap
        auto* buffer = new uint8_t[imageWidth*imageHeight*4];
        memset( buffer , 0 , imageWidth*imageHeight*4 );
        // create an array to save the character widths
        int* widths = new int[128];
        // we need to find the character that goes below the baseline by the biggest value
        int maxUnderBaseline = 0;
        for (int i = 32 ; i < 127 ; ++i) {
            fontFace.initGlyph(i, FT_LOAD_DEFAULT);
            // get the glyph metrics
            const auto& glyphMetrics = fontFace.get()->glyph->metrics;
            // find the character that reaches below the baseline by the biggest value
            int glyphHang = (glyphMetrics.horiBearingY-glyphMetrics.height)/64;
            if (glyphHang < maxUnderBaseline) {
                maxUnderBaseline = glyphHang;
            }
        }
        // draw all characters
        for (int i = 0 ; i < 128 ; ++i) {
            fontFace.initGlyph(i, FT_LOAD_DEFAULT);
            // convert to an anti-aliased bitmap
            fontFace.renderGlyph(FT_RENDER_MODE_NORMAL);
            // save the character width
            widths[i] = fontFace.get()->glyph->metrics.width/64;
            // find the tile position where we have to draw the character
            int x = (i%16)*(fontSize+2);
            int y = (i/16)*(fontSize+2);
            x += 1; // 1 pixel padding from the left side of the tile
            y += (fontSize+2) - fontFace.get()->glyph->bitmap_top + maxUnderBaseline - 1;
            // draw the character
            const FT_Bitmap& bitmap = fontFace.get()->glyph->bitmap;
            for (int xx = 0 ; xx < bitmap.width ; ++xx) {
                for (int yy = 0 ; yy < bitmap.rows ; ++yy ) {
                    uint8_t r = bitmap.buffer[(yy*(bitmap.width)+xx)];
                    buffer[(y+yy)*imageWidth*4+(x+xx)*4+0] = r;
                    buffer[(y+yy)*imageWidth*4+(x+xx)*4+1] = r;
                    buffer[(y+yy)*imageWidth*4+(x+xx)*4+2] = r;
                    buffer[(y+yy)*imageWidth*4+(x+xx)*4+3] = 255;
                }
            }
        }

        int paddedSize = 0;
        bool bitmapSaved = Bitmap::write(bitmapFilename, { imageWidth, imageHeight, buffer, &paddedSize });
        if (bitmapSaved) {
            ENGINE_INFO("FontSource : Bitmap {0} save succeeded!", bitmapFilename);
        } else {
            ENGINE_ERR("FontSource : Bitmap {0} save failed!", bitmapFilename);
        }

        bool widthSaved = FileSystem::write(widthsFilename, widths, 128);
        if (widthSaved) {
            ENGINE_INFO("FontSource : Character widths {0} save succeeded!", widthsFilename);
        } else {
            ENGINE_ERR("FontSource : Character widths {0} save failed!", widthsFilename);
        }
    }

    bool Fonts::exists(const std::string& fontPath) {
        return fonts.find(fontPath) != fonts.end();
    }

}