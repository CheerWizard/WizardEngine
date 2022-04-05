// ECS
#include <ecs/Scene.h>
#include <ecs/Components.h>
// Core
#include <time/Time.h>
#include <math/Math.h>
// Graphics
#include <graphics/transform/TransformComponents.h>
#include <graphics/core/geometry/Mesh.h>

#define DLL_API __declspec(dllexport)
#define DLL_EXPORT extern "C" DLL_API

using namespace engine;
using namespace engine::core;
using namespace engine::time;
using namespace engine::math;
using namespace engine::graphics;