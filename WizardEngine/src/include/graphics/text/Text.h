//
// Created by mecha on 26.03.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/core/math/Projections.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>

namespace engine {

    struct TextComponent {
        std::string text;
        std::string font;
        shader::Vec4fUniform color = { "color", { 1, 1, 1, 1 } };
        Transform3dComponent transform;
        TextureComponent bitmap;
        float padding = 0;
        float whiteSpaceWidth = 0.1f;

        TextComponent(
                const std::string& text,
                const std::string& font,
                const std::string& bitmap,
                const Transform3dComponent& transform,
                const glm::vec4& color,
                const float& padding = 0,
                const float& whiteSpaceWidth = 0.1f
        ) : text(text), font(font),
        bitmap({ bitmap, TextureType::TEXTURE_2D, { "bitmap", 1 } }),
        transform(transform), color({ "color", color }),
        padding(padding), whiteSpaceWidth(whiteSpaceWidth) {}
    };

    class TextView : public Entity {

    public:
        TextView(
                const std::string& tag,
                EntityContainer* container,
                const TextComponent& textComponent
        )
        : Entity(tag, container) {
            init(textComponent);
        }

    private:
        void init(const TextComponent& textComponent) {
            add<TextComponent>(textComponent);
        }
    };

}