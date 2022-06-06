//
// Created by mecha on 24.09.2021.
//

#pragma once

#include <yaml/yaml.h>

namespace engine::graphics {

    using namespace core;

    typedef array<u32> IndexData;

    IndexData copy(const IndexData &indexData);
}