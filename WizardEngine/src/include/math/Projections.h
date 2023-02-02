//
// Created by mecha on 25.09.2021.
//

#pragma once

#include <serialization/serialization.h>

#define DEFAULT_FIELD_OF_VIEW 60.0f
#define DEFAULT_Z_NEAR 0.01f
#define DEFAULT_Z_FAR 1000.0f

namespace engine::math {

    struct ENGINE_API OrthographicMatrix : shader::Mat4fUniform {
        float left = -1;
        float right = 1;
        float bottom = -1;
        float top = 1;

        OrthographicMatrix() {
            name = "projection";
        }

        OrthographicMatrix(
                const char* name,
                const float left,
                const float right,
                const float bottom,
                const float top
        ) : left(left), right(right), bottom(bottom), top(top) {
            this->name = name;
        }

        OrthographicMatrix(
                const float left,
                const float right,
                const float bottom,
                const float top
        ) : left(left), right(right), bottom(bottom), top(top) {
            name = "projection";
        }

        void apply();
    };

    struct ENGINE_API PerspectiveMatrix : shader::Mat4fUniform {
        float aspectRatio = 1024.0f / 768.0f;
        float fieldOfView = DEFAULT_FIELD_OF_VIEW;
        float zNear = DEFAULT_Z_NEAR;
        float zFar = DEFAULT_Z_FAR;

        PerspectiveMatrix() {
            name = "projection";
        }

        void apply();
    };

    void serialize(YAML::Emitter& out, const char* key, const OrthographicMatrix& projection);
    void deserialize(const YAML::Node& parent, const char* key, OrthographicMatrix& projection);
    void serialize(YAML::Emitter& out, const char* key, const PerspectiveMatrix& projection);
    void deserialize(const YAML::Node& parent, const char* key, PerspectiveMatrix& projection);

    void write(std::fstream& file, OrthographicMatrix& value);
    void read(std::fstream& file, OrthographicMatrix& value);
    void write(std::fstream& file, PerspectiveMatrix& value);
    void read(std::fstream& file, PerspectiveMatrix& value);
}