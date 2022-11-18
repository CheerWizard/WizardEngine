out flat int f_id;
out vec2 f_uv;

#include include/multiple.glsl

void update(int circleId) {
    vec4 circlePos = transform[circleId] * vec4(position, 1.0);
    gl_Position = camera * circlePos;
    f_id = circleId;
    f_uv = uv;
}

void update(float circleId) {
    update(int(circleId));
}