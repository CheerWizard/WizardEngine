//
// Created by mecha on 24.08.2022.
//

#include <graphics/materials/Color.h>

namespace engine::graphics {

    ShaderScript colorScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            u32 i = 0;
            registry.each<ColorComponent>([&shader, &i](ColorComponent* colorComponent) {
                shader.setUniformArrayElement(i++, colorComponent->color);
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto colorComponent = entity.get<ColorComponent>();
            if (colorComponent) {
                shader.setUniform(colorComponent->color);
            }
        };

        return script;
    }

    void ColorComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "ColorComponent";
        out << YAML::BeginMap;
        yaml::serialize(out, "color", color);
        out << YAML::EndMap;
    }

    void ColorComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["ColorComponent"];
        if (root) {
            yaml::deserialize(root, "color", color);
        }
    }

    void ColorComponent::read(std::fstream &file) {
        ::read(file, color.value);
    }

    void ColorComponent::write(std::fstream &file) {
        ::write(file, color.value);
    }

}