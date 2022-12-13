//
// Created by mecha on 05.10.2022.
//

#pragma once

#include <ecs/Entity.h>

using namespace engine::ecs;
using namespace engine::time;

class Scriptable {

public:
    virtual ~Scriptable() = default;

public:
    virtual void onCreate() {}
    virtual void onUpdate(Time dt) {}
    virtual void onDestroy() {}
    virtual void serialize(YAML::Emitter& out) {}
    virtual void deserialize(const YAML::Node& parent) {}

public:
    Entity root;
};

typedef Scriptable* (*newScriptable) ();

#define INIT_SCRIPT(name) \
extern "C" SCRIPT_API Scriptable* _cdecl new##name() { \
    return new name(); \
} \
