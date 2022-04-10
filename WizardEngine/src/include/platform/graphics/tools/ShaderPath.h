//
// Created by mecha on 29.01.2022.
//

#pragma once

#include <string>

#define ENGINE_SHADERS_PATH engine::io::getEngineShadersPath()
#define RUNTIME_SHADERS_PATH engine::io::getRuntimeShadersPath()
#define GRAPHICS_CORE_VERSION engine::io::getGraphicsCoreVersion()

namespace engine::io {
    std::string getRuntimeShadersPath();
    std::string getEngineShadersPath();
    const char* getGraphicsCoreVersion();
}
