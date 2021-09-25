//
// Created by mecha on 25.09.2021.
//

#pragma once

#include "Uniform.h"

#define DEFAULT_FIELD_OF_VIEW 60.0f
#define DEFAULT_Z_NEAR 0.01f
#define DEFAULT_Z_FAR 1000.0f

namespace engine {

    struct OrthographicMatrix : Mat4fUniform {
        float left;
        float right;
        float bottom;
        float top;

        OrthographicMatrix(const char* name,
                           const float &left = -1.0f,
                           const float &right = 1.0f,
                           const float &bottom = -1.0f,
                           const float &top = 1.0f) :
                           Mat4fUniform(name),
                           top(top),
                           bottom(bottom),
                           left(left),
                           right(right) {}

    public:
        const OrthographicMatrix& applyChanges() override;

    };

    struct PerspectiveMatrix : Mat4fUniform {
        float fieldOfView;
        float aspectRatio;
        float zNear;
        float zFar;

        PerspectiveMatrix(const char* name,
                          const float &aspectRatio,
                          const float &fieldOfView = DEFAULT_FIELD_OF_VIEW,
                          const float &zNear = DEFAULT_Z_NEAR,
                          const float &zFar = DEFAULT_Z_FAR) :
                          fieldOfView(fieldOfView),
                          aspectRatio(aspectRatio),
                          zNear(zNear),
                          zFar(zFar),
                          Mat4fUniform(name) {}

    public:
        const PerspectiveMatrix& applyChanges() override;

    };


}