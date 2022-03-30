//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <graphics/core/shader/Uniform.h>

#define DEFAULT_VIEW_POS_3D {0, 0, 1}
#define DEFAULT_VIEW_ROT_3D {0, 0, 0}
#define DEFAULT_VIEW_POS_2D {0, 0, 1}

using namespace engine::shader;

namespace engine {

    struct ViewMatrix3d : Mat4fUniform {
        Vec3fUniform position = { "viewPosition", DEFAULT_VIEW_POS_3D };
        glm::vec3 rotation = DEFAULT_VIEW_ROT_3D;
        float scale = 1;

        ViewMatrix3d() {
            name = "view";
        }
    };

    struct ViewMatrix2d : Mat4fUniform {
        glm::vec3 position = DEFAULT_VIEW_POS_2D;
        float rotation = 0;

        ViewMatrix2d() {
            name = "view";
        }
    };

    class ViewMatrices {

    private:
        ViewMatrices() = default;
        ~ViewMatrices() = default;

    public:
        static void update(ViewMatrix2d &viewMatrix2D);
        static void update(ViewMatrix3d &viewMatrix3D);

    };

}