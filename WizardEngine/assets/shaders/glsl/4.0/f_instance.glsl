#version 400 core

in flat int f_id;
in vec2 f_uv;
in vec3 f_objPos;
in vec3 f_normal;

out vec4 fragment;

uniform sampler2D diffuseSampler[32];

struct AmbientLight {
    float strength;
    vec3 color;
};
uniform AmbientLight ambientLight;

struct DiffuseLight {
    vec3 position;
    vec3 color;
};
uniform DiffuseLight diffuseLight;

uniform vec3 viewPosition;

struct SpecularLight {
    vec3 color;
    vec3 position;
    float strength;
};
uniform SpecularLight specularLight;

void main() {
    // object color
    vec4 objColor = texture(diffuseSampler[f_id], f_uv);
    // ambient calculation
    vec3 ambient = ambientLight.strength * ambientLight.color;
    // diffuse calculation
    vec3 normal = normalize(f_normal);
    vec3 lightDir = normalize(diffuseLight.position - f_objPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseLight.color;
    // specular calculation
    vec3 specularLightDir = normalize(specularLight.position - f_objPos);
    vec3 viewDir = normalize(viewPosition - f_objPos);
    vec3 reflectDir = reflect(-specularLightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularLight.strength * spec * specularLight.color;

    vec4 lights = vec4(ambient, 1.0) + vec4(diffuse, 1.0) + vec4(specular, 1.0);
    fragment = lights * objColor;
}