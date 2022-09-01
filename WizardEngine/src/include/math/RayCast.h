//
// Created by mecha on 21.08.2022.
//

#pragma once

#include <math/ViewProjections.h>

namespace engine::math {

    class RayCast final {

    public:
        static vec3f toWorldSpace(
                const vec2f& screenCoords,
                u32 width, u32 height,
                const ViewProjection3d& vp
        );
        static vec2f toNormalizedDeviceSpace(const vec2f& screenCoords, u32 width, u32 height);
        static vec4f toClipSpace(const vec2f& normalizedDeviceCoords);
        static vec4f toEyeSpace(const vec4f& clipCoords, const mat4f& projectionMatrix);
        static vec3f toWorldSpace(const vec4f& eyeCoords, const mat4f& viewMatrix);
    };

}
