//
// Created by mecha on 28.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include <core/map.h>

#define FONTS engine::io::Fonts::get()
#define FONT_EXISTS(fontId) FONTS.exists(fontId)
#define FONT_ABSENT(fontId) !FONT_EXISTS(fontId)
#define GET_FONT(fontId) FONTS.getFont(fontId)
#define GET_CHARACTER(fontId, c) FONTS.getCharacter(fontPath, c)

namespace engine::graphics {

    struct CharVertex {
        glm::vec2 position = { 0.5, 0.5 };
        glm::vec2 uv = { 0, 0 };
    };

    typedef BatchVertex<CharVertex> BatchCharVertex;

    struct Character {
        graphics::VertexDataComponent<BatchCharVertex> vertexDataComponent;
        glm::vec2 size = { 0, 0 };
        glm::vec2 bearing = { 0, 0 };
        float advance = 0;
    };

    typedef std::unordered_map<char, Character> Characters;
}

namespace engine::io {

    using namespace core;

    class Fonts {

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
        std::unordered_map<u32, graphics::Characters> fonts;
    };

}