//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "../shader/Uniform.h"

#include "ViewMatrix.h"
#include "ProjectionMatrix.h"

namespace engine {

    struct ViewProjection2d : Mat4fUniform {
        ViewMatrix2d viewMatrix;
        OrthographicMatrix orthographicMatrix;

        ViewProjection2d(const char* name,
                         const ViewMatrix2d &viewMatrix,
                         const OrthographicMatrix &orthographicMatrix
        ) : orthographicMatrix(orthographicMatrix), viewMatrix(viewMatrix), Mat4fUniform(name) {
            applyChanges();
        }

    public:
        ViewProjection2d& applyChanges() override;

    };

    struct ViewProjection3d : Mat4fUniform {
        ViewMatrix3d viewMatrix;
        PerspectiveMatrix perspectiveMatrix;

        ViewProjection3d(const char* name,
                         const ViewMatrix3d &viewMatrix,
                         const PerspectiveMatrix &perspectiveMatrix
        ) : perspectiveMatrix(perspectiveMatrix), viewMatrix(viewMatrix), Mat4fUniform(name) {
            applyChanges();
        }

    public:
        ViewProjection3d& applyChanges() override;

    };

}