out vec4 f_color;

#include include/multiple.glsl

void update(int quadId) {
    vec4 quadPos = transform[quadId] * vec4(position, 1.0);
    gl_Position = camera * quadPos;
    f_color = color;
}

void update(float quadId) {
    update(int(quadId));
}