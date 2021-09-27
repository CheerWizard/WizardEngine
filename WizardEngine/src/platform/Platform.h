//
// Created by mecha on 30.08.2021.
//

#pragma once

#ifdef WIN32
/* Windows x64/x86 */
	#ifdef WIN64
	/* Windows x64  */
		#define PLATFORM_WINDOWS
	#else
	/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
	/* We also have to check __ANDROID__ before __linux__
     * since android is based on the linux kernel
     * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif

#ifdef GLFW

    #define INIT_WINDOW(props) createScope<WindowsWindow>(props)
    #define INIT_INPUT createScope<WindowsInput>()

	#define GLFW_WINDOW static_cast<GLFWwindow*>(Application::getInstance().getWindow()->getNativeWindow())

	#define GET_WINDOW_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).windowCallback
    #define GET_KEYBOARD_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).keyboardCallback
    #define GET_MOUSE_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).mouseCallback
    #define GET_CURSOR_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).cursorCallback

    #ifdef GL
        #define GLFW_INCLUDE_NONE
        #define INIT_GRAPHICS_CONTEXT createScope<GLContext>(GLFW_WINDOW)

    #elif defined(VK)
        #error "Vulkan graphics context is not supported!"

    #endif

#endif