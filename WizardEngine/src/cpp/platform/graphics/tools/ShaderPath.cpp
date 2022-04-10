//
// Created by mecha on 29.01.2022.
//

#include <platform/graphics/tools/ShaderPath.h>

namespace engine::io {

    std::string getRuntimeShadersPath() {
        return "assets/shaders/glsl/4.0";
    }

    std::string getEngineShadersPath() {
        return "../WizardEngine/assets/shaders/glsl/4.0";
    }

    const char* getGraphicsCoreVersion() {
        return "#version 400";
    }

}