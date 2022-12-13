//
// Created by mecha on 21.05.2022.
//

#pragma once

#include <sstream>
#include <yaml/yaml.h>

#define serializable() \
ENGINE_API void serialize(YAML::Emitter& out); \
ENGINE_API void deserialize(const YAML::Node& parent);

#define decl_serializable() \
void serialize(YAML::Emitter& out); \
void deserialize(const YAML::Node& parent);