const float shinyMax = 32;

vec3 spec(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float shiny) {
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(viewDir, reflectDir), 0.0), shinyMax - shiny) * lightColor;
}

vec3 spec(vec3 lightColor, vec3 lightPos, vec3 pos, vec3 normal, vec3 viewDir, float shiny) {
    vec3 lightDir = normalize(lightPos - pos);
    return spec(lightColor, lightDir, normal, viewDir, shiny);
}