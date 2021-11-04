struct AmbientLight {
    float strength;
    vec3 color;
};
uniform AmbientLight ambientLight;

vec3 amb3() {
    return ambientLight.strength * ambientLight.color;
}

vec4 amb4() {
    return vec4(amb3(), 1.0);
}