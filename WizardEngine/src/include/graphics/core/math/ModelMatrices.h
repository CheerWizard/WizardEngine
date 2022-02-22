//
// Created by mecha on 23.09.2021.
//

#pragma once

#include <graphics/core/shader/Uniform.h>

#define DEFAULT_TRANSLATION_3D {0.5, 0.5, 0.5}
#define DEFAULT_ROTATION_3D {0, 0, 0}
#define DEFAULT_SCALE_3D {1, 1, 1}

#define DEFAULT_TRANSLATION_2D {0.5, 0.5}
#define DEFAULT_ROTATION_2D 0
#define DEFAULT_SCALE_2D {1, 1}

namespace engine {

    struct ModelMatrix2d;
    struct ModelMatrix3d;

    class ModelMatrices final {

    private:
        ModelMatrices() = default;
        ~ModelMatrices() = default;

    public:
        static void update(ModelMatrix2d &modelMatrix2D);
        static void update(ModelMatrix3d &modelMatrix3D);

    };

    struct ModelMatrix3d : shader::Mat4fUniform {
        glm::fvec3 position = DEFAULT_TRANSLATION_3D;
        glm::fvec3 rotation = DEFAULT_ROTATION_3D;
        glm::fvec3 scale = DEFAULT_SCALE_3D;

        ModelMatrix3d() = default;

        ModelMatrix3d(
                const glm::vec3& pos,
                const glm::vec3& rot,
                const glm::vec3& scale
        ) : position(pos), rotation(rot), scale(scale) {
        }
    };

    struct ModelMatrix2d : shader::Mat4fUniform {
        glm::fvec2 position = DEFAULT_TRANSLATION_2D;
        float rotation = DEFAULT_ROTATION_2D;
        glm::fvec2 scale = DEFAULT_SCALE_2D;

        ModelMatrix2d() = default;

        ModelMatrix2d(
                const glm::vec2& pos,
                const float& rot,
                const glm::vec2& scale
        ) : position(pos), rotation(rot), scale(scale) {
        }
    };

}