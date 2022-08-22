//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <yaml/yaml.h>

#define DEFAULT_VIEW_POS_3D {0, 0, -1}
#define DEFAULT_VIEW_POS_2D {0, 0, 1}

using namespace engine::shader;

namespace engine::math {

    struct ViewMatrix3d : Mat4fUniform {
        Vec3fUniform position = { "viewPosition", DEFAULT_VIEW_POS_3D };
        float pitch = 0;
        float yaw = 0;
        float roll = 0;
        float scale = 1;

        ViewMatrix3d() {
            name = "view";
        }

        glm::quat orientation() const;
        vec3f forwardDirection() const;
        vec3f upDirection() const;
        vec3f rightDirection() const;
    };

    struct ViewMatrix2d : Mat4fUniform {
        vec3f position = DEFAULT_VIEW_POS_2D;
        float rotation = 0;

        ViewMatrix2d() {
            name = "view";
        }
    };

    void update(ViewMatrix2d &viewMatrix2D);
    void update(ViewMatrix3d &viewMatrix3D);

    void serialize(YAML::Emitter& out, const char* key, const ViewMatrix2d& vm);
    void deserialize(const YAML::Node& parent, const char* key, ViewMatrix2d& vm);

    void serialize(YAML::Emitter& out, const char* key, const ViewMatrix3d& vm);
    void deserialize(const YAML::Node& parent, const char* key, ViewMatrix3d& vm);
}