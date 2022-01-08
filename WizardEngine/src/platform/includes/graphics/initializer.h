//
// Created by mecha on 02.01.2022.
//

#pragma once

#ifdef GL
#include "../../opengl/GraphicsInitializer.h"
#elif defined(VK)
#error "Vulkan graphics context is not supported!"
#endif