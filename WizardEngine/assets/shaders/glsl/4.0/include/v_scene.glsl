out flat int f_id;
out vec3 f_pos;
out vec2 f_uv;
out vec3 f_normal;
out vec3 f_tangent;
out vec3 f_bitangent;

#include multiple.glsl

void updateObject(int objectId) {
    f_id = objectId;
    vec4 worldPos = camera * transform[objectId] * vec4(position, 1.0);
    f_pos = worldPos.xyz;
    f_uv = uv;
    // calculate TBN matrix
    mat3 model = mat3(transform[objectId]);
    f_normal = normalize(model * normal);
    f_tangent = normalize(model * tangent);
    f_bitangent = normalize(model * bitangent);
    // re-orthogonalize T with respect to N
    f_tangent = normalize(f_tangent - dot(f_tangent, f_normal) * f_normal);

    gl_Position = worldPos;
}

void updateObject(float objectId) {
    updateObject(int(objectId));
}