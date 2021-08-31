//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "../platform/Platform.h"

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef DEBUG
	#ifdef PLATFORM_WINDOWS
		#define DEBUGBREAK() __debugbreak()
	#elif defined(PLATFORM_LINUX)
		#include <signal.h>
		#define DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define ENABLE_ASSERTS
    #define ENABLE_LOGS
#else
	#define DEBUGBREAK()
#endif