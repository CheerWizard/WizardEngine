#include diffuse.glsl
#include specular.glsl

struct PhongLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

vec3 phong(
    PhongLight phongLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir,
    float shiny
) {
    vec3 lightDir = normalize(phongLight.position.xyz - pos);
    vec3 diff = diff(phongLight.diffuse.xyz, lightDir, normal);
    vec3 spec = spec(phongLight.specular.xyz, lightDir, normal, viewDir, shiny);
    return phongLight.ambient.xyz + diff + spec;
}

struct DirectLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

vec3 dir(
    DirectLight directLight,
    vec3 normal,
    vec3 viewDir,
    float shiny
) {
    vec3 lightDir = normalize(-directLight.direction.xyz);
    vec3 diff = diff(directLight.diffuse.xyz, lightDir, normal);
    vec3 spec = spec(directLight.specular.xyz, lightDir, normal, viewDir, shiny);
    return directLight.ambient.xyz + diff + spec;
}

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
};

vec3 point(
    PointLight pointLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir,
    float shiny
) {
    // calculate distance and luminosity
    float distance = length(pointLight.position.xyz - pos);
    float attenuation = pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance);
    float luminosity = 1.0 / attenuation;
    pointLight.ambient *= luminosity;
    pointLight.linear *= luminosity;
    pointLight.specular *= luminosity;
    // convert to phong light
    PhongLight phongLight = PhongLight(
        pointLight.position,
        pointLight.ambient,
        pointLight.diffuse,
        pointLight.specular
    );
    return phong(phongLight, pos, normal, viewDir, shiny);
}

/**
   position - position of light
   direction - direction of spot
*/
struct FlashLight {
    vec4 position;
    vec4 direction;
    float cutoff;
    float outerCutoff;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
};

vec3 flash(
    FlashLight flashLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir,
    float shiny
) {
    // calculate spot light
    vec3 lightDir = normalize(flashLight.position.xyz - pos);
    float theta = dot(lightDir, normalize(-flashLight.direction.xyz));
    float epsilon = flashLight.cutoff - flashLight.outerCutoff;
    float intensity = clamp((theta - flashLight.outerCutoff) / epsilon, 0.0, 1.0);
    flashLight.diffuse *= intensity;
    flashLight.specular *= intensity;
    // check if we are in range of spot light
    if (theta > flashLight.cutoff) {
        // convert to point light
        PointLight pointLight = PointLight(
            flashLight.position,
            flashLight.ambient,
            flashLight.diffuse,
            flashLight.specular,
            flashLight.constant,
            flashLight.linear,
            flashLight.quadratic
        );
        return point(pointLight, pos, normal, viewDir, shiny);
    } else {
        // return ambient light to make pixels not completely dark!
        return flashLight.ambient.xyz;
    }
}