#pragma once

#include <math/ViewMatrices.h>
#include <math/Projections.h>

namespace engine::math {

    struct ENGINE_API ViewProjection2d : Mat4fUniform {
        ViewMatrix2d viewMatrix;
        OrthographicMatrix orthographicMatrix;

        ViewProjection2d() {
            apply();
        }

        void apply();
    };

    struct ENGINE_API ViewProjection3d : Mat4fUniform {
        ViewMatrix3d viewMatrix;
        PerspectiveMatrix perspectiveMatrix;

        ViewProjection3d() {
            apply();
        }

        ViewProjection3d(const float& aspectRatio) {
            perspectiveMatrix.aspectRatio = aspectRatio;
            apply();
        }

        void apply();
    };

    void serialize(YAML::Emitter& out, const char* key, const ViewProjection2d& vp);
    void deserialize(const YAML::Node& parent, const char* key, ViewProjection2d& vp);
    void serialize(YAML::Emitter& out, const char* key, const ViewProjection3d& vp);
    void deserialize(const YAML::Node& parent, const char* key, ViewProjection3d& vp);

    void write(std::fstream& file, ViewProjection2d& vp);
    void read(std::fstream& file, ViewProjection2d& vp);
    void write(std::fstream& file, ViewProjection3d& vp);
    void read(std::fstream& file, ViewProjection3d& vp);
}