//
// Created by mecha on 05.11.2021.
//

#include "ShaderController.h"

namespace engine {

    void ShaderController::updateFUniform(
            const Ref<Shader> &shader,
            Mat4fUniform &uniform,
            const uint32_t &uniformDataIndex
    ) {
        if (shader->hasFUniformBlocks()) {
            if (!uniform.isUpdated) return;
            uniform.isUpdated = false;
            shader->updateFUniformBuffer(UniformData { uniform.toFloatPtr(), uniformDataIndex });
        } else {
            shader->setUniform(uniform);
        }
    }

    void ShaderController::updateVUniform(
            const Ref<Shader> &shader,
            Mat4fUniform &uniform,
            const uint32_t &uniformDataIndex
    ) {
        if (shader->hasVUniformBlocks()) {
            if (!uniform.isUpdated) return;
            uniform.isUpdated = false;
            shader->updateVUniformBuffer(UniformData { uniform.toFloatPtr(), uniformDataIndex });
        } else {
            shader->setUniform(uniform);
        }
    }

}