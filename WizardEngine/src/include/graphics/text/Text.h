//
// Created by mecha on 26.03.2022.
//

#pragma once

#include <graphics/core/shader/Uniform.h>
#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/text/FontSource.h>
#include <ecs/Entity.h>

namespace engine {

    struct TextVertex {
        glm::vec2 position = { 0.5, 0.5 };
        glm::vec2 uv = { 0.5, 0.5 };
    };

    struct TextComponent {
        std::string text;
        shader::Vec3fUniform color = { "color", { 1, 1, 1 } };
        float size;
        glm::vec2 position = { 1, 1 };
    };

    class TextView : public Entity {

    public:
        TextView(const std::string& tag, EntityContainer* container, const TextComponent& textComponent)
        : Entity(tag, container) {
            init(textComponent);
        }

    private:
        void init(const TextComponent& textComponent) {
            add<VertexDataComponent<TextVertex>>(initSquare(textComponent));
            add<TextComponent>(textComponent);
        }

        VertexDataComponent<TextVertex> initSquare(const TextComponent& textComponent) {
            // iterate through all characters
            std::string::const_iterator c;
            auto& text = textComponent.text;
            auto& size = textComponent.size;
            auto& position = textComponent.position;
            for (c = text.begin(); c != text.end(); c++) {
                auto& charView = CHAR_VIEW(*c);
                float x = position.x + charView.bearing.x * size;
                float y = position.y - (charView.size.y - charView.bearing.y) * size;
                float w = charView.size.x * size;
                float h = charView.size.y * size;
                // update VBO for each character
                float vertices[6][4] = {
                        { x,     y + h, 0.0f, 0.0f },
                        { x,     y,     0.0f, 1.0f },
                        { x + w, y,     1.0f, 1.0f },

                        { x,     y + h, 0.0f, 0.0f },
                        { x + w, y,     1.0f, 1.0f },
                        { x + w, y + h, 1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.textureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                position.x += (charView.advance >> 6) * size; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
        }
    };

    using namespace shader;
    ShaderScript textShaderScript();

}