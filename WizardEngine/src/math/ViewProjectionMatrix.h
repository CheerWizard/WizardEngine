//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "Uniform.h"

#define DEFAULT_FIELD_OF_VIEW 60.0f
#define DEFAULT_Z_NEAR 0.01f
#define DEFAULT_Z_FAR 1000.0f
#define DEFAULT_POSITION glm::vec3(1.0f)
#define DEFAULT_ROTATION 0
#define DEFAULT_SCALE 1.0f

namespace engine {

    struct ViewProjectionMatrix : Mat4fUniform {
        glm::fvec3 position;
        float rotation;
        float scale;
        Mat4fUniform projectionMatrix;

        ViewProjectionMatrix(const char* name,
                             const Mat4fUniform &projectionMatrix,
                             const glm::fvec3 &position = DEFAULT_POSITION,
                             const float &rotation = DEFAULT_ROTATION,
                             const float &scale = DEFAULT_SCALE) :
                             position(position),
                             rotation(rotation),
                             scale(scale),
                             projectionMatrix(projectionMatrix),
                             Mat4fUniform(name) {}

    public:
        const glm::fmat4& applyChanges() override;

    };

    struct OrthographicMatrix : Mat4fUniform {
        float left;
        float right;
        float bottom;
        float top;

        OrthographicMatrix(const char* name,
                           const float &left = 0,
                           const float &right = 0,
                           const float &bottom = 0,
                           const float &top = 0) :
                           Mat4fUniform(name),
                           top(top),
                           bottom(bottom),
                           left(left),
                           right(right) {}

    public:
        const glm::fmat4& applyChanges() override;

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
        const glm::fmat4& applyChanges() override;

    };

}