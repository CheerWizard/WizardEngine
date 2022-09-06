//
// Created by mecha on 30.08.2022.
//

#include <graphics/core/Screen.h>
#include <math/Projections.h>

namespace engine::graphics {

    void ScreenRenderer::init() {
        shaderProgram = shader::BaseShaderProgram(
                io::ShaderProps {
                        "screen",
                        "primitive_quad.glsl",
                        "f_screen.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
    }

    void ScreenRenderer::onWindowResized(u32 width, u32 height) {
//        shaderProgram.start();
//        math::OrthographicMatrix projection;
//        projection.apply();
//        shaderProgram.setUniform(projection);
//        shaderProgram.stop();
    }

}