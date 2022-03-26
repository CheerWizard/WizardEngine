//
// Created by mecha on 26.03.2022.
//

#include <graphics/text/FontSource.h>

namespace engine {

    void FontSource::init() {
        if (FT_Init_FreeType(&ftLibrary)) {
            ENGINE_CRIT("FreeType : library init() failed!");
        } else {
            ENGINE_INFO("FreeType : library init() succeeded!");
        }
    }

    bool FontSource::load(const std::string &fontPath) {
        bool isLoaded = FT_New_Face(ftLibrary, "fonts/arial.ttf", 0, &ftFace) == 0;

        if (isLoaded) {
            ENGINE_INFO("FreeType : load font {0} succeeded!", fontPath);
        } else {
            ENGINE_ERR("FreeType : failed to load font {0}", fontPath);
        }

        return isLoaded;
    }

    bool FontSource::loadGlyph(const uint8_t& glyph) {
        bool isLoaded = FT_Load_Char(ftFace, glyph, FT_LOAD_RENDER) == 0;

        if (isLoaded) {
            ENGINE_INFO("FreeType : load glyph {0} succeeded!", glyph);
        } else {
            ENGINE_ERR("FreeType : failed to load glyph {0}", glyph);
        }

        return isLoaded;
    }

    void FontSource::loadChars(const size_t &fontSize) {
        FT_Set_Pixel_Sizes(ftFace, 0, fontSize);
        TextureBuffer::disableByteAlignment();

        for (uint8_t ch = 0; ch < 128; ch++) {
            // load character glyph
            if (!loadGlyph(ch)) {
                continue;
            }
            // generate texture
            TextureBuffer texture = TextureBuffer(TextureType::TEXTURE_2D);
            texture.bind();
            texture.load({
                 static_cast<int>(ftFace->glyph->bitmap.width),
                 static_cast<int>(ftFace->glyph->bitmap.rows),
                 CHANNEL_RED,
                 ftFace->glyph->bitmap.buffer
             });
            texture.setParams({
                  { TextureParamName::MIN_FILTER, TextureParamValue::LINEAR },
                  { TextureParamName::MAG_FILTER, TextureParamValue::LINEAR },
                  { TextureParamName::WRAP_S, TextureParamValue::CLAMP_TO_EDGE },
                  { TextureParamName::WRAP_T, TextureParamValue::CLAMP_TO_EDGE },
            });
            // now store character for later use
            auto charView = CharView {
                    texture,
                    { ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows },
                    { ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top },
                    ftFace->glyph->advance.x
            };
            chars.insert(std::pair<char, CharView>(ch, charView));
        }

        clear();
    }

    void FontSource::clear() {
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLibrary);
    }

    const CharView &FontSource::getCharView(const char &ch) {
        return chars[ch];
    }

}