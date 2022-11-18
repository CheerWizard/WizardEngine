//
// Created by mecha on 07.09.2022.
//

// dynamic library
// defined for library
#ifdef ENGINE_DLL_BUILD
#define ENGINE_API __declspec(dllexport)
// defined for client
#elif defined(ENGINE_DLL)
#define ENGINE_API __declspec(dllimport)
#else
// static library
#define ENGINE_API
#endif

// dynamic library
// defined for library
#ifdef SCRIPT_DLL
#define SCRIPT_API __declspec(dllexport)
// defined for client
#else
#define SCRIPT_API __declspec(dllimport)
#endif