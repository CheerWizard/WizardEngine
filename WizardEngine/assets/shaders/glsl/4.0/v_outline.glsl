#include include/multiple.glsl

void update(int objId) {
    vec4 pos = transform[objId] * vec4(position, 1.0);
    gl_Position = camera * pos;
}

void update(float objId) {
    update(int(objId));
}