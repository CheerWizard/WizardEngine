out vec2 f_uv;
out flat int f_id;
out flat int f_slot;
out vec3 f_pos;
out vec3 f_normal;

#include multiple.glsl

void updateObject(int id) {
    f_id = id;
    vec4 objPos = transform[id] * vec4(position, 1.0);

    f_pos = objPos.xyz;
    f_uv = uv;
    f_normal = normal;
    f_slot = int(textureSlot);

    gl_Position = camera * objPos;
}

void updateObject(float id) {
    updateObject(int(id));
}