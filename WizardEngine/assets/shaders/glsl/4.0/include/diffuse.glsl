struct DiffuseLight {
    vec3 position;
    vec3 color;
};
uniform DiffuseLight diffuseLight;

// pos - not normalized vector position of object in scene
// normal - normalized vector normal of object in scene
vec3 diff3(vec3 pos, vec3 normal) {
    vec3 lightDir = normalize(diffuseLight.position - pos);
    float diff = max(dot(normal, lightDir), 0.0);
    return diff * diffuseLight.color;
}

// objPos - not normalized vector position of object in scene
// objNormal - normalized vector normal of object in scene
vec4 diff4(vec3 objPos, vec3 objNormal) {
    return vec4(diff3(objPos, objNormal), 1.0);
}