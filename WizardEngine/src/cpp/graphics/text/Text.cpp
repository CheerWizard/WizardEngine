//
// Created by mecha on 26.03.2022.
//

#include <graphics/text/Text.h>

namespace engine::graphics {

    void TextProjection::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "TextProjection";
        math::serialize(out, "viewProjection", viewProjection);
        out << YAML::EndMap;
    }

    void TextProjection::deserialize(const YAML::Node &parent) {
        auto root = parent["TextProjection"];
        if (root) {
            math::deserialize(root, "viewProjection", viewProjection);
        }
    }

    ShaderScript textProjectionScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            registry.each<TextProjection>([&shader](TextProjection* textProjection) {
                shader.setUniform(textProjection->viewProjection);
            });
        };

        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto textProjection = entity.get<TextProjection>();
            if (textProjection) {
                shader.setUniform(textProjection->viewProjection);
            }
        };

        return script;
    }

}