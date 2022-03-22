out vec4 f_color;

#include include/multiple.glsl

void update(int lineId) {
    vec4 linePos = transform[lineId] * vec4(position, 1.0);
    gl_Position = camera * linePos;
    f_color = color;
}

void update(float lineId) {
    update(int(lineId));
}
