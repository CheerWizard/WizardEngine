//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "Uniform.h"

#include "TransformMatrix.h"
#include "ProjectionMatrix.h"

namespace engine {

    struct ViewProjection2d : Mat4fUniform {
        TransformMatrix transformMatrix;
        OrthographicMatrix orthographicMatrix;

        ViewProjection2d(const char* name,
                             const TransformMatrix &transformMatrix,
                             const OrthographicMatrix &orthographicMatrix) :
                             orthographicMatrix(orthographicMatrix),
                             transformMatrix(transformMatrix),
                             Mat4fUniform(name) {
        }

    public:
        const ViewProjection2d& applyChanges() override;
    };

    struct ViewProjection3d : Mat4fUniform {
        TransformMatrix transformMatrix;
        PerspectiveMatrix perspectiveMatrix;

        ViewProjection3d(const char* name,
                         const TransformMatrix &transformMatrix,
                         const PerspectiveMatrix &perspectiveMatrix) :
                         perspectiveMatrix(perspectiveMatrix),
                         transformMatrix(transformMatrix),
                         Mat4fUniform(name) {
        }

    public:
        const ViewProjection3d& applyChanges() override;

    };

}