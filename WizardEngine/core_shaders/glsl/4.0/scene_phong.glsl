#version 400 core

#include include/f_scene.glsl
#include material/material.glsl

layout(location = 1) out vec4 brightColor;
layout(location = 2) out int uuid;

const uint LIGHT_COUNT = 8;
uniform PhongLight phongLight[LIGHT_COUNT];

uniform Material material[2];

uniform int uuids[2];

void main() {
    for (uint i = 0 ; i < LIGHT_COUNT ; i++) {
        PhongLight light = phongLight[i];
        vec3 pos = f_pos;
        vec2 uv = f_uv;
        vec3 normal = f_normal;
        vec3 tangent = f_tangent;
        vec3 bitangent = f_bitangent;
        vec3 viewPos = viewPosition;

        light.ambient *= material[getId()].ambient;
        light.diffuse *= material[getId()].diffuse;
        light.specular *= material[getId()].specular;

        vec3 lightPos = light.position;
        mat3 TBN = transpose(mat3(tangent, bitangent, normal));
        // apply TBN matrix
        pos *= TBN;
        viewPos *= TBN;
        lightPos *= TBN;

        vec3 lightDir = normalize(lightPos - pos);
        vec3 viewDir = normalize(viewPos - pos);

        if (material[getId()].enableParallaxMap) {
            uv = parallax(
                material[getId()].depthSlot, uv, viewDir,
                material[getId()].heightScale,
                material[getId()].minLayers, material[getId()].maxLayers
            );
            // UV bounds should be [0, 1]
            // discard pixels that are out of bounds [0, 1]
            if (uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) {
                discard;
            }
        }

        if (material[getId()].enableNormalMap) {
            // apply normal map
            normal = texture(material[getId()].normalSlot, uv).rgb;
            normal = normalize(normal * 2.0 - 1.0);
            normal = normalize(TBN * normal);
        }

        vec3 albedo = material[getId()].color.rgb;
        vec3 amb = vec3(light.ambient);
        vec3 diff = diff(light.color.xyz, lightDir, normal) * light.diffuse;
        vec3 specular;
        if (material[getId()].enableBlinn) {
            specular = specBlinn(light.color.xyz, lightDir, normal, viewDir, material[getId()].shiny);
        } else {
            specular = spec(light.color.xyz, lightDir, normal, viewDir, material[getId()].shiny);
        }
        specular *= light.specular;

        if (material[getId()].enableAlbedoMap) {
            albedo = texture(material[getId()].albedoSlot, uv).rgb;
        }

        if (material[getId()].enableDiffuseMap) {
            vec3 diffMap = texture(material[getId()].diffuseSlot, uv).rgb;
            amb *= diffMap;
            diff *= diffMap;
        }

        if (material[getId()].enableSpecularMap) {
            vec3 specMap = texture(material[getId()].specularSlot, uv).rgb;
            specular *= specMap;
        }

        // calculate PBR surface model

        float metallic = material[getId()].metallicSlot;
        float roughness = material[getId()].roughness;
        float ao = material[getId()].ao;

        if (material[getId()].enableMetallicMap) {
            metallic = texture(material[getId()].metallicSlot, uv).r;
        }

        if (material[getId()].enableRoughnessMap) {
            roughness = texture(material[getId()].roughnessSlot, uv).r;
        }

        if (material[getId()].enableAOMap) {
            ao = texture(material[getId()].aoSlot, uv).r;
        }

        vec3 color = amb * (albedo + diff + specular);
        fragment += vec4(gamma(color.rgb, material[getId()].gamma), material[getId()].color.w);
    }

    float brightness = dot(fragment.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > material[getId()].brightness) {
        brightColor = vec4(fragment.rgb, 1);
    } else {
        brightColor = vec4(0, 0, 0, 1);
    }

    uuid = uuids[getId()];
}