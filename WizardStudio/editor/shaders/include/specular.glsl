const float shinyMax = 32;

vec3 spec(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float shiny) {
    vec3 reflectDir = reflect(-lightDir, normal);
    return pow(max(dot(viewDir, reflectDir), 0.0), shinyMax - shiny) * lightColor;
}

vec3 specBlinn(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float shiny) {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfwayDir), 0.0), shinyMax - shiny) * lightColor;
}