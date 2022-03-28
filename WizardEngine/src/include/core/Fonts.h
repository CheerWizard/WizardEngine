//
// Created by mecha on 28.03.2022.
//

#pragma once

#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#define FONTS engine::Fonts::get()
#define FONT_LOAD(fontPath) FONTS.load(fontPath)
#define FONT_LOAD_CHARS(fontSize) FONTS.loadChars(fontSize)
#define CHAR_VIEW(ch) FONT_SOURCE.getCharView(ch)
#define FONTS_PATH "assets/fonts"
#define FONTS_GENERATE_BITMAP(fontPath, fontSize, bmpPath, widthsPath) FONTS.generateBitmap(fontPath, fontSize, bmpPath, widthsPath)

namespace engine {

    template<typename T>
    struct vec2 {
        T x,y;
    };

    struct FontGlyph {
        vec2<uint32_t> size {};
        vec2<int> bearing {};
        long advance = 0;
    };

    class FontFace {

    public:
        FontFace() = default;

        FontFace(const FT_Face& face, const size_t& fontSize) : face(face) {
            initGlyphs(fontSize);
        }
        ~FontFace() {
            destroy();
        }

    public:
        void initGlyphs(const size_t& fontSize);
        bool initGlyph(const uint8_t& glyph, const FT_Int32& flag);
        bool renderGlyph(const FT_Render_Mode_& renderMode);
        bool setFontSize(const size_t& fontSize);
        void destroy();
        const FT_Face& get();

    private:
        FT_Face face = nullptr;
        // key - char symbol
        // value - visual presentation of char symbol
        std::unordered_map<char, FontGlyph> glyphs;
    };

    class Fonts {

    private:
        Fonts() {
            init();
        }

    public:
        Fonts(const Fonts&) = delete;
        Fonts& operator=(const Fonts &) = delete;
        Fonts(Fonts &&) = delete;
        Fonts& operator=(Fonts &&) = delete;

    public:
        static auto& get() {
            static Fonts instance;
            return instance;
        }

    public:
        bool load(const std::string& fontPath, const size_t& fontSize);
        void generateBitmap(
                const std::string& fontFilename,
                const int& fontSize,
                const std::string& bitmapFilename,
                const std::string& widthsFilename
        );

    private:
        bool exists(const std::string& fontPath);
        void init();
        void clear();

    private:
        FT_Library ftLibrary;
        // key - font file name
        // value - actual font face
        std::unordered_map<std::string, FontFace> fonts;
    };

}