const float shinyMax = 32;
const float PI = 3.14159265359;

vec3 spec(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float shiny) {
    float energyConserve = ( 2.0 + shiny ) / ( 2.0 * PI );
    vec3 reflectDir = reflect(-lightDir, normal);
    return energyConserve * pow(max(dot(viewDir, reflectDir), 0.0), shinyMax - shiny) * lightColor;
}

vec3 specBlinn(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float shiny) {
    float energyConserve = ( 8.0 + shiny ) / ( 8.0 * PI );
    vec3 halfwayDir = normalize(lightDir + viewDir);
    return energyConserve * pow(max(dot(normal, halfwayDir), 0.0), shinyMax - shiny) * lightColor;
}