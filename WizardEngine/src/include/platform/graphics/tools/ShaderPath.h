//
// Created by mecha on 29.01.2022.
//

#pragma once

#include <string>

#define ENGINE_SHADERS_PATH getEngineShadersPath()
#define RUNTIME_SHADERS_PATH getRuntimeShadersPath()
#define GRAPHICS_CORE_VERSION getGraphicsCoreVersion()

namespace engine {
    std::string getRuntimeShadersPath();
    std::string getEngineShadersPath();
    const char* getGraphicsCoreVersion();
}
