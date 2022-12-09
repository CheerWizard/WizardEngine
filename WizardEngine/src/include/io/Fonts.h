//
// Created by mecha on 28.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <core/map.h>

#define FONTS engine::io::Fonts::get()
#define FONT_EXISTS(fontId) FONTS.exists(fontId)
#define FONT_ABSENT(fontId) !FONT_EXISTS(fontId)
#define GET_FONT(fontId) FONTS.getFont(fontId)
#define GET_CHARACTER(fontId, c) FONTS.getCharacter(fontPath, c)

namespace engine::graphics {

    using namespace math;

    struct ENGINE_API CharVertex {
        vec2f position = { 0.5, 0.5 };
        vec2f uv = { 0, 0 };
    };

    typedef BatchVertex<CharVertex> BatchCharVertex;

    struct ENGINE_API Character {
        graphics::VertexDataComponent<BatchCharVertex> vertexDataComponent;
        vec2f size = { 0, 0 };
        vec2f bearing = { 0, 0 };
        float advance = 0;
    };

    typedef unordered_map<char, Character> Characters;
}

namespace engine::io {

    using namespace core;

    class ENGINE_API Fonts {

    private:
        Fonts() = default;

    public:
        Fonts(const Fonts &) = delete;
        Fonts &operator=(const Fonts &) = delete;
        Fonts(Fonts &&) = delete;
        Fonts &operator=(Fonts &&) = delete;

    public:
        static auto &get() {
            static Fonts instance;
            return instance;
        }

    public:
        u32 create(
                const char* fontPath,
                const u32& fontSize,
                const char* bitmapPath,
                const char* widthsPath
        );
        graphics::Characters& getFont(const u32& id);
        graphics::Character& getCharacter(const u32& id, const char& c);
        void clear();
        bool exists(const u32& id);

    private:
        // key - font texture buffer id
        // value - font characters map
        unordered_map<u32, graphics::Characters> fonts;
    };

}