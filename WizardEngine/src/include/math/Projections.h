//
// Created by mecha on 25.09.2021.
//

#pragma once

#include <graphics/core/shader/Uniform.h>

#define DEFAULT_FIELD_OF_VIEW 60.0f
#define DEFAULT_Z_NEAR 0.01f
#define DEFAULT_Z_FAR 1000.0f

namespace engine::math {

    struct OrthographicMatrix : shader::Mat4fUniform {
        float left = -1;
        float right = 1;
        float bottom = -1;
        float top = 1;

        OrthographicMatrix() {
            name = "projection";
        }

        OrthographicMatrix(
                const char* name,
                const float& left,
                const float& right,
                const float& bottom,
                const float& top
        ) : left(left), right(right), bottom(bottom), top(top) {
            this->name = name;
        }

        OrthographicMatrix(
                const float& left,
                const float& right,
                const float& bottom,
                const float& top
        ) : left(left), right(right), bottom(bottom), top(top) {
            name = "projection";
        }
    };

    struct PerspectiveMatrix : shader::Mat4fUniform {
        float aspectRatio = 1;
        float fieldOfView = DEFAULT_FIELD_OF_VIEW;
        float zNear = DEFAULT_Z_NEAR;
        float zFar = DEFAULT_Z_FAR;

        PerspectiveMatrix() {
            name = "projection";
        }
    };

    class Projections final {

    private:
        Projections() = default;
        ~Projections() = default;

    public:
        static void update(OrthographicMatrix &orthographicMatrix);
        static void update(PerspectiveMatrix &perspectiveMatrix);

    };

}