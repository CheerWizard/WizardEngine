#include include/multiple.glsl

out vec4 f_color;

struct Outline {
    vec4 color;
    float thickness;
};

uniform Outline outline[128];

void update(int objId) {
    Outline o = outline[objId];
    mat4 scaleMat = mat4(
        vec4(1 + o.thickness, 0, 0, 0),
        vec4(0, 1 + o.thickness, 0, 0),
        vec4(0, 0, 1 + o.thickness, 0),
        vec4(0, 0, 0, 1)
    );
    vec4 pos = camera * transform[objId] * vec4(position + normal * o.thickness, 1.0);
    gl_Position = pos;
    f_color = o.color;
}

void update(float objId) {
    update(int(objId));
}