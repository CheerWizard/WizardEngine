//
// Created by mecha on 28.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include <unordered_map>

#define FONTS engine::Fonts::get()
#define FONT_EXISTS(fontPath) FONTS.exists(fontPath)
#define FONT_ABSENT(fontPath) !FONT_EXISTS(fontPath)
#define GET_FONT(fontPath) FONTS.getFont(fontPath)
#define GET_CHARACTER(fontPath, c) FONTS.getCharacter(fontPath, c)

namespace engine {

    struct CharVertex {
        glm::vec2 position = { 0.5, 0.5 };
        glm::vec2 uv = { 0, 0 };
    };

    typedef BatchVertex<CharVertex> BatchCharVertex;

    struct Character {
        VertexDataComponent<BatchCharVertex> vertexDataComponent;
        glm::vec2 size = { 0, 0 };
        glm::vec2 bearing = { 0, 0 };
        float advance = 0;
    };

    typedef std::unordered_map<char, Character> Characters;

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
        bool create(
                const std::string& fontPath,
                const uint32_t& fontSize,
                const std::string& bitmapPath,
                const std::string& widthsPath
        );
        Characters& getFont(const std::string& fontPath);
        Character& getCharacter(const std::string& fontPath, const char& c);
        void clear();
        bool exists(const std::string& fontPath);

    private:
        // key - font file path
        // value - characters map
        std::unordered_map<std::string, Characters> fonts;
    };

}