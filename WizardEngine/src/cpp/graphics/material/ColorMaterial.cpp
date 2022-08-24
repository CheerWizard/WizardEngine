//
// Created by mecha on 24.08.2022.
//

#include <graphics/materials/ColorMaterial.h>

namespace engine::graphics {

    ShaderScript colorMaterialScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            u32 i = 0;
            registry.each<ColorMaterial>([&shader, &i](ColorMaterial* colorMaterial) {
                shader.setUniformArrayElement(i++, colorMaterial->color);
            });
        };

        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto colorMaterial = entity.get<ColorMaterial>();
            if (colorMaterial) {
                shader.setUniform(colorMaterial->color);
            }
        };

        return script;
    }

}