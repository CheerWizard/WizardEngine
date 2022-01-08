//
// Created by mecha on 17.11.2021.
//

#pragma once

#ifdef GL
#include "../../opengl/commands/RenderCommands.h"
#elif defined(VK)
#error "Vulkan graphics context is not supported!"
#endif