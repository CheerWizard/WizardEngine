struct PhongLight {
    vec3 color;
    vec3 position;
    float ambientStrength;
    float specularStrength;
};
uniform PhongLight phongLight;

vec3 phong3(vec3 pos, vec3 normal, vec3 viewPos, float shiny) {
    // amb
    vec3 amb3 = phongLight.color * phongLight.ambientStrength;
    // diff
    vec3 lightDir = normalize(phongLight.position - pos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diff3 = diff * phongLight.color;
    // spec
    vec3 specularLightDir = normalize(phongLight.position - pos);
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-specularLightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiny);
    vec3 spec3 = phongLight.specularStrength * spec * phongLight.color;
    // phong
    return amb3 + diff3 + spec3;
}

vec4 phong4(vec3 pos, vec3 normal, vec3 viewPos, float shiny) {
    return vec4(phong3(pos, normal, viewPos, shiny), 1.0);
}