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

}