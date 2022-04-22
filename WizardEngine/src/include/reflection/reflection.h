//
// Created by mecha on 21.04.2022.
//

#pragma once

#include <core/primitives.h>
#include <omp_llvm.h>

#define CLASS class __attribute__((annotate("reflect-class")))
#define STRUCT struct __attribute__((annotate("reflect-struct")))
#define PROPERTY __attribute__((annotate("reflect-property")))

namespace engine::reflection {
}