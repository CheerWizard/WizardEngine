//
// Created by mecha on 26.03.2022.
//

#pragma once

#include <core/Memory.h>
#include <platform/graphics/TextureBuffer.h>

#include <glm/glm.hpp>
#include <ft2build.h>
#include <unordered_map>
#include FT_FREETYPE_H

#define FONT_SOURCE engine::FontSource::get()
#define FONT_LOAD(fontPath) FONT_SOURCE.load(fontPath)
#define FONT_LOAD_CHARS(fontSize) FONT_SOURCE.loadChars(fontSize)
#define CHAR_VIEW(ch) FONT_SOURCE.getCharView(ch)
#define FONTS_PATH "assets/fonts"

namespace engine {

    struct CharView {
        TextureBuffer texture;
        glm::ivec2 size;
        glm::ivec2 bearing;
        long advance;
    };

    class FontSource {

    private:
        FontSource() {
            init();
        }

    public:
        FontSource(const FontSource&) = delete;
        FontSource& operator=(const FontSource &) = delete;
        FontSource(FontSource &&) = delete;
        FontSource& operator=(FontSource &&) = delete;

    public:
        static auto& get() {
            static FontSource instance;
            return instance;
        }

    public:
        bool load(const std::string& fontPath);
        void loadChars(const size_t& fontSize);
        const CharView& getCharView(const char& ch);

    private:
        void init();
        bool loadGlyph(const uint8_t& glyph);
        void clear();

    private:
        FT_Library ftLibrary;
        FT_Face ftFace;
        std::unordered_map<char, CharView> chars;

    };

}
