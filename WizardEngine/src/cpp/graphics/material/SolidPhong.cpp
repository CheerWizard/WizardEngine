//
// Created by mecha on 24.08.2022.
//

#include <graphics/materials/SolidPhong.h>

namespace engine::graphics {

    ShaderScript solidPhongScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            u32 i = 0;
            registry.each<SolidPhong>([&shader, &i](SolidPhong* solidPhong) {
                shader.setUniformArrayStructField(i, solidPhong->name, solidPhong->color);
                shader.setUniformArrayStructField(i, solidPhong->name, solidPhong->ambient);
                shader.setUniformArrayStructField(i, solidPhong->name, solidPhong->diffuse);
                shader.setUniformArrayStructField(i, solidPhong->name, solidPhong->specular);
                shader.setUniformArrayStructField(i, solidPhong->name, solidPhong->shiny);
                i++;
            });
        };

        script.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto solidPhong = entity.get<SolidPhong>();
            if (solidPhong) {
                shader.setUniformStructField(solidPhong->name, solidPhong->color);
                shader.setUniformStructField(solidPhong->name, solidPhong->ambient);
                shader.setUniformStructField(solidPhong->name, solidPhong->diffuse);
                shader.setUniformStructField(solidPhong->name, solidPhong->specular);
                shader.setUniformStructField(solidPhong->name, solidPhong->shiny);
            }
        };

        return script;
    }

}