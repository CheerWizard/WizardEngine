//
// Created by mecha on 21.08.2022.
//

#include <math/RayCast.h>

namespace engine::math {

    vec3f RayCast::toWorldSpace(
            const vec2f &screenCoords,
            u32 width, u32 height,
            const ViewProjection3d& vp
    ) {
        vec2f normalizedDeviceCoords = toNormalizedDeviceSpace(screenCoords, width, height);
        vec4f clipCoords = toClipSpace(normalizedDeviceCoords);
        vec4f eyeCoords = toEyeSpace(clipCoords, vp.perspectiveMatrix.value);
        return toWorldSpace(eyeCoords, vp.viewMatrix.value);
    }

    vec2f RayCast::toNormalizedDeviceSpace(const vec2f &screenCoords, u32 width, u32 height) {
        f32 x = (2.0f * screenCoords.x()) / static_cast<f32>(width) - 1;
        f32 y = (2.0f * screenCoords.y()) / static_cast<f32>(height) - 1;
        return { x, y };
    }

    vec4f RayCast::toClipSpace(const vec2f &normalizedDeviceCoords) {
        return { normalizedDeviceCoords, -1.0f, 1.0f };
    }

    vec4f RayCast::toEyeSpace(const vec4f &clipCoords, const mat4f& projectionMatrix) {
        mat4f p = projectionMatrix;
        glm::mat4 m;

        m[0][0] = p.v0[0];
        m[0][1] = p.v0[1];
        m[0][2] = p.v0[2];
        m[0][3] = p.v0[3];

        m[1][0] = p.v1[0];
        m[1][1] = p.v1[1];
        m[1][2] = p.v1[2];
        m[1][3] = p.v1[3];

        m[2][0] = p.v2[0];
        m[2][1] = p.v2[1];
        m[2][2] = p.v2[2];
        m[2][3] = p.v2[3];

        m[3][0] = p.v3[0];
        m[3][1] = p.v3[1];
        m[3][2] = p.v3[2];
        m[3][3] = p.v3[3];

        auto inverseM = glm::inverse(m);
        mat4f inverseP;
        inverseP.v0 = { inverseM[0][0], inverseM[0][1], inverseM[0][2], inverseM[0][3] };
        inverseP.v1 = { inverseM[1][0], inverseM[1][1], inverseM[1][2], inverseM[1][3] };
        inverseP.v2 = { inverseM[2][0], inverseM[2][1], inverseM[2][2], inverseM[2][3] };
        inverseP.v3 = { inverseM[3][0], inverseM[3][1], inverseM[3][2], inverseM[3][3] };

        vec4f eyeCoords = inverseP * clipCoords;

        return { eyeCoords.x(), eyeCoords.y(), -1.0f, 0 };
    }

    vec3f RayCast::toWorldSpace(const vec4f &eyeCoords, const mat4f &viewMatrix) {
        mat4f v = viewMatrix;
        glm::mat4 m;

        m[0][0] = v.v0[0];
        m[0][1] = v.v0[1];
        m[0][2] = v.v0[2];
        m[0][3] = v.v0[3];

        m[1][0] = v.v1[0];
        m[1][1] = v.v1[1];
        m[1][2] = v.v1[2];
        m[1][3] = v.v1[3];

        m[2][0] = v.v2[0];
        m[2][1] = v.v2[1];
        m[2][2] = v.v2[2];
        m[2][3] = v.v2[3];

        m[3][0] = v.v3[0];
        m[3][1] = v.v3[1];
        m[3][2] = v.v3[2];
        m[3][3] = v.v3[3];

        auto inverseM = glm::inverse(m);
        mat4f inverseV;
        inverseV.v0 = { inverseM[0][0], inverseM[0][1], inverseM[0][2], inverseM[0][3] };
        inverseV.v1 = { inverseM[1][0], inverseM[1][1], inverseM[1][2], inverseM[1][3] };
        inverseV.v2 = { inverseM[2][0], inverseM[2][1], inverseM[2][2], inverseM[2][3] };
        inverseV.v3 = { inverseM[3][0], inverseM[3][1], inverseM[3][2], inverseM[3][3] };

        vec4f worldCoords = (inverseV * eyeCoords);
        return { worldCoords.x(), worldCoords.y(), worldCoords.z() };
    }

}