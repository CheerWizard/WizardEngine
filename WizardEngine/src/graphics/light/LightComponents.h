//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../core/shader/Uniform.h"

namespace engine {

    struct AmbientLightComponent {
        Vec3fUniform color;
        FloatUniform strength;
    };

    class LightComponents {

    private:
        LightComponents() = default;
        ~LightComponents() = default;

    public:
        inline static AmbientLightComponent newAmbient() {
            return newAmbient({0.5, 0.5, 0.5}, 0.5);
        }

        inline static AmbientLightComponent newAmbient(const glm::vec3 &color, const float &strength) {
            return {
                    { "color", color },
                    { "strength", strength }
            };
        }
    };

}