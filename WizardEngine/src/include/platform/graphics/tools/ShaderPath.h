 //
// Created by mecha on 29.01.2022.
//

#pragma once

#include <string>
#include <core/core.h>

#define ENGINE_SHADERS_PATH engine::io::getEngineShadersPath()
#define RUNTIME_SHADERS_PATH engine::io::getRuntimeShadersPath()
#define GRAPHICS_CORE_VERSION engine::io::getGraphicsCoreVersion()

namespace engine::io {
    ENGINE_API std::string getRuntimeShadersPath();
    ENGINE_API std::string getEngineShadersPath();
    ENGINE_API const char* getGraphicsCoreVersion();
}
