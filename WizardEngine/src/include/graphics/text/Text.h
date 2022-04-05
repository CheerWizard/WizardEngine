//
// Created by mecha on 26.03.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <math/Projections.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    struct TextComponent {
        std::string text;
        std::string font;
        shader::Vec4fUniform color = { "color", { 1, 1, 1, 1 } };
        Transform3dComponent transform;
        TextureComponent bitmap;
        float paddingX = 0;
        float paddingY = 0;
        float whiteSpaceWidth = 0.02f;
        FloatUniform transparency = { "transparency", 0.5f };
        bool isUpdated = true; // flag should be updated, once this text has been changed

        TextComponent(
                const std::string& text,
                const std::string& font,
                const std::string& bitmap,
                const Transform3dComponent& transform,
                const glm::vec4& color,
                const float& paddingX = 0,
                const float& paddingY = 0,
                const float& whiteSpaceWidth = 0.02f,
                const float& transparency = 0.5f
        ) : text(text), font(font),
        bitmap({ bitmap, TextureType::TEXTURE_2D, { "bitmap", 1 } }),
        transform(transform), color({ "color", color }),
        paddingX(paddingX), paddingY(paddingY),
        whiteSpaceWidth(whiteSpaceWidth), transparency({ "transparency", 0.5f }) {}
    };

    // just separate types for ecs as we want to process text 2D/3D separately
    struct Text2d : TextComponent {
        Text2d(
                const std::string& text,
                const std::string& font,
                const std::string& bitmap,
                const Transform3dComponent& transform,
                const glm::vec4& color,
                const float& padding = 0,
                const float& whiteSpaceWidth = 0.02f
        ) : TextComponent(text, font, bitmap, transform, color, padding, whiteSpaceWidth) {}
    };

    struct Text3d : TextComponent {
        Text3d(
                const std::string& text,
                const std::string& font,
                const std::string& bitmap,
                const Transform3dComponent& transform,
                const glm::vec4& color,
                const float& padding = 0,
                const float& whiteSpaceWidth = 0.02f
        ) : TextComponent(text, font, bitmap, transform, color, padding, whiteSpaceWidth) {}
    };

    struct TextProjection : Camera3dComponent {
        TextProjection(const float& aspectRatio) : Camera3dComponent(aspectRatio) {}
    };

    class Text2dView : public Entity {

    public:
        Text2dView(
                const std::string& tag,
                EntityContainer* container,
                const Text2d& text,
                const float& aspectRatio
        ) : Entity(tag, container) {
            add<Text2d>(text);
            auto textProjection = TextProjection(aspectRatio);
            textProjection.viewMatrix.position.value = {0, 0, -1};
            math::ViewProjections::update(textProjection);
            add<TextProjection>(textProjection);
        }
    };

    class Text3dView : public Entity {

    public:
        Text3dView(
                const std::string& tag,
                EntityContainer* container,
                const Text3d& text
        ) : Entity(tag, container) {
            add<Text3d>(text);
        }
    };

    using namespace shader;
    ShaderScript textProjectionScript();

}