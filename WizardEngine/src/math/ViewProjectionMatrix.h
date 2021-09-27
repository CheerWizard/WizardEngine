//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "Uniform.h"

#include "ViewMatrix.h"
#include "ProjectionMatrix.h"

namespace engine {

    struct ViewProjection2d : Mat4fUniform {
        ViewMatrix viewMatrix;
        OrthographicMatrix orthographicMatrix;

        ViewProjection2d(const char* name,
                             const ViewMatrix &viewMatrix,
                             const OrthographicMatrix &orthographicMatrix) :
                             orthographicMatrix(orthographicMatrix),
                             viewMatrix(viewMatrix),
                             Mat4fUniform(name) {
        }

    public:
        const ViewProjection2d& applyChanges() override;
    };

    struct ViewProjection3d : Mat4fUniform {
        ViewMatrix viewMatrix;
        PerspectiveMatrix perspectiveMatrix;

        ViewProjection3d(const char* name,
                         const ViewMatrix &viewMatrix,
                         const PerspectiveMatrix &perspectiveMatrix) :
                         perspectiveMatrix(perspectiveMatrix),
                         viewMatrix(viewMatrix),
                         Mat4fUniform(name) {
        }

    public:
        const ViewProjection3d& applyChanges() override;

    };

}