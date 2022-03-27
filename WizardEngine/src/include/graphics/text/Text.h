//
// Created by mecha on 26.03.2022.
//

#pragma once

#include <graphics/core/shader/Uniform.h>
#include <graphics/core/buffer_data/VertexData.h>
#include <platform/graphics/TextureBuffer.h>
#include <ecs/Entity.h>
#include <core/Memory.h>
#include <graphics/core/math/Projections.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <unordered_map>

#define FONT_SOURCE engine::FontSource::get()
#define FONT_LOAD(fontPath) FONT_SOURCE.load(fontPath)
#define FONT_LOAD_CHARS(fontSize) FONT_SOURCE.loadChars(fontSize)
#define CHAR_VIEW(ch) FONT_SOURCE.getCharView(ch)
#define FONTS_PATH "assets/fonts"

namespace engine {

    struct TextVertex {
        glm::vec2 position = { 0.5, 0.5 };
        glm::vec2 uv = { 0.5, 0.5 };
    };

    // visual presentation of char symbol
    struct FontGlyph {
        VertexDataComponent<TextVertex> quad;
        glm::vec2 size {};
        glm::vec2 bearing {};
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

    struct TextComponent {
        std::string string;
        shader::Vec3fUniform color = { "color", { 1, 1, 1 } };
        glm::vec2 position = { 0, 0 };
        float size = 0;

        TextComponent() = default;
        TextComponent(const std::string& string,
                      const Vec3fUniform& color,
                      const glm::vec2& position,
                      const float& size)
        : string(string), color(color), position(position), size(size) {
            update();
        }

        void update();
    };

    struct StaticTextProjection : OrthographicMatrix {};

    class TextView : public Entity {

    public:
        TextView(const std::string& tag, EntityContainer* container, const TextComponent& textComponent)
        : Entity(tag, container) {
            init(textComponent);
        }

    private:
        void init(const TextComponent& textComponent) {
            add<TextComponent>(textComponent);
        }
    };

}