//
// Created by mecha on 29.01.2022.
//

#pragma once

#include <Windows.h>
#define LIB_TYPE HINSTANCE
#define LOAD_LIB(libpath) LoadLibrary(libpath)
#define LOAD_FUNC(lib, fn) GetProcAddress((lib), (fn))
#define FREE_LIB(lib) FreeLibrary(lib)