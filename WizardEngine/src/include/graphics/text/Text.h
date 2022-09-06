//
// Created by mecha on 26.03.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <math/Projections.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/texture/Texture.h>
#include <graphics/camera/CameraComponents.h>
#include <platform/graphics/TextureBuffer.h>

namespace engine::graphics {

    using namespace math;

    serialize_template_component(TextComponent, T) {
        std::string text;
        Vec4fUniform color = { "color", { 1, 1, 1, 1 } };
        Transform3dComponent transform;
        TextureComponent bitmap;
        float paddingX = 0;
        float paddingY = 0;
        float whiteSpaceWidth = 0.02f;
        FloatUniform transparency = { "transparency", 0.5f };
        bool isUpdated = true; // flag should be updated, once this text has been changed

        TextComponent() = default;

        TextComponent(
                const u32& textureId,
                const std::string& text,
                const Transform3dComponent& transform,
                const vec4f& color,
                const float& paddingX = 0,
                const float& paddingY = 0,
                const float& whiteSpaceWidth = 0.02f,
                const float& transparency = 0.5f
        ) : text(text), bitmap({ textureId, TextureBuffer::getTypeId(TextureType::TEXTURE_2D), { "bitmap", 0 } }),
        transform(transform), color({ "color", color }),
        paddingX(paddingX), paddingY(paddingY),
        whiteSpaceWidth(whiteSpaceWidth), transparency({ "transparency", 0.5f }) {}

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

    template<typename T>
    void TextComponent<T>::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "TextComponent";
        yaml::serialize(out, "text", text);
        yaml::serialize(out, "color", color);
        transform.serialize(out);
        bitmap.serialize(out);
        yaml::serialize(out, "paddingX", paddingX);
        yaml::serialize(out, "paddingY", paddingY);
        yaml::serialize(out, "whiteSpaceWidth", whiteSpaceWidth);
        yaml::serialize(out, "transparency", transparency);
        out << YAML::EndMap;
    }

    template<typename T>
    void TextComponent<T>::deserialize(const YAML::Node &parent) {
        auto root = parent["TextComponent"];
        if (root) {
            yaml::deserialize(root, "text", text);
            yaml::deserialize(root, "color", color);
            transform.deserialize(root);
            bitmap.deserialize(root);
            yaml::deserialize(root, "paddingX", paddingX);
            yaml::deserialize(root, "paddingY", paddingY);
            yaml::deserialize(root, "whiteSpaceWidth", whiteSpaceWidth);
            yaml::deserialize(root, "transparency", transparency);
        }
    }

    struct Text2d : TextComponent<Text2d> {
        Text2d(
                const u32& textureId,
                const std::string& text,
                const Transform3dComponent& transform,
                const vec4f& color,
                const float& paddingX = 0,
                const float& paddingY = 0,
                const float& whiteSpaceWidth = 0.02f,
                const float& transparency = 0.5f
        ) : TextComponent<Text2d>(textureId, text, transform, color, paddingX, paddingY, whiteSpaceWidth, transparency) {}
    };

    struct Text3d : TextComponent<Text3d> {
        Text3d(
                const u32& textureId,
                const std::string& text,
                const Transform3dComponent& transform,
                const vec4f& color,
                const float& paddingX = 0,
                const float& paddingY = 0,
                const float& whiteSpaceWidth = 0.02f,
                const float& transparency = 0.5f
        ) : TextComponent<Text3d>(textureId, text, transform, color, paddingX, paddingY, whiteSpaceWidth, transparency) {}
    };

    serialize_component(TextProjection) {
        math::ViewProjection3d viewProjection;
        TextProjection(float aspectRatio) {
            viewProjection = math::ViewProjection3d(aspectRatio);
        }

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

    class Text2dView : public ecs::Entity {

    public:
        Text2dView(
                const std::string& tag,
                ecs::EntityContainer* container,
                const Text2d& text,
                const float& aspectRatio
        ) : Entity(tag, container) {
            add<Text2d>(text);
            auto textProjection = TextProjection(aspectRatio);
            textProjection.viewProjection.viewMatrix.position.value = {0, 0, -1};
            textProjection.viewProjection.apply();
            add<TextProjection>(textProjection);
        }
    };

    class Text3dView : public ecs::Entity {

    public:
        Text3dView(
                const std::string& tag,
                ecs::EntityContainer* container,
                const Text3d& text
        ) : Entity(tag, container) {
            add<Text3d>(text);
        }
    };

    using namespace shader;
    ShaderScript textProjectionScript();

}