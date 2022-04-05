//
// Created by mecha on 26.03.2022.
//

#include <graphics/text/Text.h>

namespace engine::graphics {

    ShaderScript textProjectionScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto vps = registry.view<TextProjection>();
            for (auto [entity, vp] : vps.each()) {
                shader.setUniform(vp);
            }
        };

        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto vp = entity.getPtr<TextProjection>();
            if (vp) {
                shader.setUniform(*vp);
            }
        };

        return script;
    }

}