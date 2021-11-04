struct SpecularLight {
    vec3 color;
    vec3 position;
    float strength;
};
uniform SpecularLight specularLight;

// pos - not normalized vector position of object in scene
// normal - normalized vector normal of object in scene
vec3 spec3(vec3 pos, vec3 normal, vec3 viewPos, float shiny) {
    vec3 specularLightDir = normalize(specularLight.position - pos);
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-specularLightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiny);
    return specularLight.strength * spec * specularLight.color;
}

// pos - not normalized vector position of object in scene
// normal - normalized vector normal of object in scene
vec4 spec4(vec3 pos, vec3 normal, vec3 viewPos, float shiny) {
    return vec4(spec3(pos, normal, viewPos, shiny), 1.0);
}