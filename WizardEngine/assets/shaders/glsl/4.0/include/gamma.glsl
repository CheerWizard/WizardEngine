vec3 gamma(vec3 color, float g) {
    return pow(color, vec3(1.0 / g));
}