//
// Created by mecha on 01.04.2022.
//

#include <graphics/core/buffer_data/VertexData.h>
#include <graphics/core/shader/Uniform.h>
#include <graphics/core/texture/Texture.h>

namespace engine::graphics {

    using namespace math;
    using namespace shader;

    struct ScreenVertex {
        vec2f position = { -1, -1 };
        vec2f uv = { 0, 0 };
    };

    struct ScreenSettings {
        ColorFormat colorFormat = ColorFormat::RGBA8;
        u32 samples = 1;
        FloatUniform exposure = { "exposure", 1 };
        FloatUniform gamma = { "gamma", 2.2 };
        BoolUniform enableHDR = { "enableHDR", false };

        ScreenSettings() = default;
        ScreenSettings(
                const ColorFormat& colorFormat, u32 samples,
                float exposure, float gamma, bool enableHDR
        ) : colorFormat(colorFormat), samples(samples) {
            this->gamma.value = gamma;
            this->exposure.value = exposure;
            this->enableHDR.value = enableHDR;
        }
    };

    struct Screen : VertexDataComponent<ScreenVertex> {
        Screen();
    };
}