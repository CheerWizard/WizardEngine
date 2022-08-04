//
// Created by mecha on 23.09.2021.
//

#pragma once

#include <yaml/yaml.h>

#define DEFAULT_TRANSLATION_3D {0.5, 0.5, 0.5}
#define DEFAULT_ROTATION_3D {0, 0, 0}
#define DEFAULT_SCALE_3D {1, 1, 1}

#define DEFAULT_TRANSLATION_2D {0.5, 0.5}
#define DEFAULT_ROTATION_2D 0
#define DEFAULT_SCALE_2D {1, 1}

namespace engine::math {

    struct ModelMatrix2d;
    struct ModelMatrix3d;

    void updateModel2d(ModelMatrix2d &modelMatrix2D);
    void updateModel3d(ModelMatrix3d &modelMatrix3D);

    struct ModelMatrix3d : shader::Mat4fUniform {
        math::vec3f position = DEFAULT_TRANSLATION_3D;
        math::vec3f rotation = DEFAULT_ROTATION_3D;
        math::vec3f scale = DEFAULT_SCALE_3D;

        ModelMatrix3d() {
            updateModel3d(*this);
        }

        ModelMatrix3d(
                const math::vec3f& pos,
                const math::vec3f& rot,
                const math::vec3f& scale
        ) : position(pos), rotation(rot), scale(scale) {
            updateModel3d(*this);
        }

        ModelMatrix3d(const ModelMatrix3d& modelMatrix3D)
        : position(modelMatrix3D.position), rotation(modelMatrix3D.rotation), scale(modelMatrix3D.scale), Uniform(modelMatrix3D) {
            updateModel3d(*this);
        }
    };

    struct ModelMatrix2d : shader::Mat4fUniform {
        math::vec2f position = DEFAULT_TRANSLATION_2D;
        float rotation = DEFAULT_ROTATION_2D;
        math::vec2f scale = DEFAULT_SCALE_2D;

        ModelMatrix2d() {
            updateModel2d(*this);
        }

        ModelMatrix2d(
                const char* name,
                const math::vec2f& pos,
                const float& rot,
                const math::vec2f& scale
        ) : position(pos), rotation(rot), scale(scale) {
            this->name = name;
            updateModel2d(*this);
        }

        ModelMatrix2d(
                const math::vec2f& pos,
                const float& rot,
                const math::vec2f& scale
        ) : position(pos), rotation(rot), scale(scale) {
            updateModel2d(*this);
        }
    };

    void serialize(YAML::Emitter& out, const char* key, const ModelMatrix2d& model);
    void deserialize(const YAML::Node& parent, const char* key, ModelMatrix2d& model);

    void serialize(YAML::Emitter& out, const char* key, const ModelMatrix3d& model);
    void deserialize(const YAML::Node& parent, const char* key, ModelMatrix3d& model);

}