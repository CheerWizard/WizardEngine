out flat int f_id;
out vec3 f_pos;
out vec2 f_uv;
out vec3 f_normal;

#include multiple.glsl

void updateObject(int objectId) {
    f_id = objectId;
    vec4 objPos = transform[objectId] * vec4(position, 1.0);

    f_pos = objPos.xyz;
    f_uv = uv;
    f_normal = normal;

    gl_Position = camera * objPos;
}

void updateObject(float objectId) {
    updateObject(int(objectId));
}