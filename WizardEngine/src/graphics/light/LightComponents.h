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

    struct DiffuseLightComponent {
        Vec3fUniform color;
        Vec3fUniform position;
    };

    struct SpecularLightComponent {
        Vec3fUniform color;
        Vec3fUniform position;
        FloatUniform strength;
    };

    class LightComponents {

    private:
        LightComponents() = default;
        ~LightComponents() = default;

    public:
        inline static AmbientLightComponent newAmbient(
                const glm::vec3 &color = {0.5f, 0.5f, 0.5f },
                const float &strength = 0.5f
        ) {
            return {
                    { "ambientLight.color", color },
                    { "ambientLight.strength", strength }
            };
        }

        inline static DiffuseLightComponent newDiffuse(
                const glm::vec3 &color = {1, 1, 1},
                const glm::vec3 &pos = {0.5, 0.5, 0.5}
        ) {
            return {
                    { "diffuseLight.color", color },
                    { "diffuseLight.position", pos }
            };
        }

        inline static SpecularLightComponent newSpecular(
                const glm::vec3 &color = {1, 1, 1},
                const glm::vec3 &pos = {0.5, 0.5, 0.5},
                const float &strength = 0.5
        ) {
            return {
                    { "specularLight.color", color },
                    { "specularLight.position", pos },
                    { "specularLight.strength", strength }
            };
        }
    };

}