layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in float instanceId;

out vec2 f_uv;
out flat int f_id;
out vec3 f_pos;
out vec3 f_normal;

layout (std140) uniform Camera {
    mat4 camera;
};

uniform mat4 transform[128];

void updateObject(int id) {
    f_id = id;
    vec4 objPos = transform[id] * vec4(position, 1.0);

    f_pos = vec3(objPos);
    f_uv = uv;
    f_normal = normal;

    gl_Position = camera * objPos;
}

void updateObject(float id) {
    updateObject(int(id));
}