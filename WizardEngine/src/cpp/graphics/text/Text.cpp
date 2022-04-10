//
// Created by mecha on 26.03.2022.
//

#include <graphics/text/Text.h>

namespace engine::graphics {

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