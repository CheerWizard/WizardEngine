//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <serialization/serialization.h>

#define DEFAULT_VIEW_POS_3D {0, 0, -1}
#define DEFAULT_VIEW_POS_2D {0, 0, 1}

using namespace engine::shader;

namespace engine::math {

    struct ENGINE_API ViewMatrix3d : Mat4fUniform {
        Vec3fUniform position = { "viewPosition", DEFAULT_VIEW_POS_3D };
        float pitch = 0;
        float yaw = 0;
        float roll = 0;
        float scale = 1;

        ViewMatrix3d() {
            name = "view";
        }

        [[nodiscard]] glm::quat orientation() const;
        [[nodiscard]] vec3f forwardDirection() const;
        [[nodiscard]] vec3f upDirection() const;
        [[nodiscard]] vec3f rightDirection() const;

        void apply();
    };

    struct ENGINE_API ViewMatrix2d : Mat4fUniform {
        vec3f position = DEFAULT_VIEW_POS_2D;
        float rotation = 0;

        ViewMatrix2d() {
            name = "view";
        }

        void apply();
    };

    void serialize(YAML::Emitter& out, const char* key, const ViewMatrix2d& vm);
    void deserialize(const YAML::Node& parent, const char* key, ViewMatrix2d& vm);
    void write(std::fstream& file, const ViewMatrix2d& value);
    void read(std::fstream& file, ViewMatrix2d& value);

    void serialize(YAML::Emitter& out, const char* key, const ViewMatrix3d& vm);
    void deserialize(const YAML::Node& parent, const char* key, ViewMatrix3d& vm);
    void write(std::fstream& file, const ViewMatrix3d& value);
    void read(std::fstream& file, ViewMatrix3d& value);
}