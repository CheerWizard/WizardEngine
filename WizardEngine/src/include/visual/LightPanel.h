//
// Created by mecha on 02.12.2022.
//

#pragma once

#include <visual/Visual.h>
#include <graphics/light/Light.h>

namespace engine::visual {

    using namespace engine::graphics;

    class ENGINE_API LightsPanel final {
    public:
        static void draw(const vector<PhongLight>& lights);
        static void draw(const vector<Light>& lights);
    };

}