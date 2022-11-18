vec3 diff(vec3 lightColor, vec3 lightDir, vec3 normal) {
    return max(dot(normal, lightDir), 0.0) * lightColor;
}

vec3 diff(vec3 lightColor, vec3 lightPos, vec3 pos, vec3 normal) {
    vec3 lightDir = normalize(lightPos - pos);
    return diff(lightColor, lightDir, normal);
}