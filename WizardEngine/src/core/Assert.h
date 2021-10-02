//
// Created by mecha on 30.08.2021.
//
#pragma once

#include "Debug.h"
#include "Logger.h"

#ifdef ENABLE_ASSERTS
    #define CLIENT_ASSERT(x, ...) { if (!(x)) { CLIENT_ERR("Assertion failed : {0}", __VA_ARGS__); DEBUGBREAK(); } }
    #define ENGINE_ASSERT(x, ...) { if (!(x)) { ENGINE_ERR("Assertion failed : {0}", __VA_ARGS__); DEBUGBREAK(); } }
#else
    #define CLIENT_ASSERT(x, ...)
    #define ENGINE_ASSERT(x, ...)
#endif
