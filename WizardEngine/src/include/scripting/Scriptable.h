//
// Created by mecha on 05.10.2022.
//

#pragma once

#include <ecs/Entity.h>

using namespace engine::ecs;
using namespace engine::time;

class Scriptable : public engine::io::Serializable {

public:
    virtual ~Scriptable() = default;

public:
    virtual void onCreate() = 0;
    virtual void onUpdate(Time dt) = 0;
    virtual void onDestroy() = 0;

public:
    Entity root;
};

typedef Scriptable* (*newScriptable) ();

#define INIT_SCRIPT(name) \
extern "C" SCRIPT_API Scriptable* _cdecl new##name() { \
    return new name(); \
} \