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

#include <glm/glm.hpp>

namespace engine {

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