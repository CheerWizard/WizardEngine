//
// Created by mecha on 26.03.2022.
//

#include <graphics/text/Text.h>

namespace engine {

    ShaderScript textShaderScript() {
        auto script = ShaderScript();

//        script.updateRegistry = [](const BaseShader& shader, entt::registry &registry) {
//            auto texts = registry.view<TextComponent>();
//            for (auto& text : texts) {
//                text.
//            }
//        };
//
//        script.updateEntity = [](const BaseShader& shader, const Entity& entity) {
//            auto texture = entity.getPtr<CubeMapTextureComponent>();
//            if (texture) {
//                shader.setUniform(texture->sampler);
//                ACTIVATE_TEXTURE(*texture);
//            }
//        };

        return script;
    }

}