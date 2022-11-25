#include ../include/diffuse.glsl
#include ../include/specular.glsl
#include ../include/gamma.glsl
#include ../include/parallax.glsl

struct Material {
    vec4 color;

    sampler2D albedoSlot;
    bool enableAlbedoMap;
    float ambient;

    sampler2D diffuseSlot;
    bool enableDiffuseMap;
    float diffuse;

    sampler2D specularSlot;
    bool enableSpecularMap;
    float specular;
    float shiny;

    bool enableBlinn;

    float gamma;

    sampler2D normalSlot;
    bool enableNormalMap;

    sampler2D depthSlot;
    bool enableParallaxMap;
    float heightScale;
    float minLayers;
    float maxLayers;

    float brightness;

    sampler2D metallicSlot;
    bool enableMetallicMap;
    float metallic;

    sampler2D roughnessSlot;
    bool enableRoughnessMap;
    float roughness;

    sampler2D aoSlot;
    bool enableAOMap;
    float ao;
};

const int MATERIAL_TEXTURE_UNITS = 5;

struct PhongLight {
    vec3 position;
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
};

vec4 applyPhong(
    Material material,
    vec3 pos, vec2 uv, vec3 normal,
    vec3 tangent, vec3 bitangent,
    vec3 viewPos,
    PhongLight light
) {
    light.ambient *= material.ambient;
    light.diffuse *= material.diffuse;
    light.specular *= material.specular;

    vec3 lightPos = light.position;
    mat3 TBN = transpose(mat3(tangent, bitangent, normal));
    // apply TBN matrix
    pos *= TBN;
    viewPos *= TBN;
    lightPos *= TBN;

    vec3 lightDir = normalize(lightPos - pos);
    vec3 viewDir = normalize(viewPos - pos);

    if (material.enableParallaxMap) {
        uv = parallax(
            material.depthSlot, uv, viewDir,
            material.heightScale, material.minLayers, material.maxLayers
        );
        // UV bounds should be [0, 1]
        // discard pixels that are out of bounds [0, 1]
        if (uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) {
            discard;
        }
    }

    if (material.enableNormalMap) {
        // apply normal map
        normal = texture(material.normalSlot, uv).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(TBN * normal);
    }

    vec3 albedo = material.color.rgb;
    vec3 amb = vec3(light.ambient);
    vec3 diff = diff(light.color.xyz, lightDir, normal) * light.diffuse;
    vec3 specular;
    if (material.enableBlinn) {
        specular = specBlinn(light.color.xyz, lightDir, normal, viewDir, material.shiny);
    } else {
        specular = spec(light.color.xyz, lightDir, normal, viewDir, material.shiny);
    }
    specular *= light.specular;

    if (material.enableAlbedoMap) {
        albedo = texture(material.albedoSlot, uv).rgb;
    }

    if (material.enableDiffuseMap) {
        vec3 diffMap = texture(material.diffuseSlot, uv).rgb;
        amb *= diffMap;
        diff *= diffMap;
    }

    if (material.enableSpecularMap) {
        vec3 specMap = texture(material.specularSlot, uv).rgb;
        specular *= specMap;
    }

    // calculate PBR surface model

    float metallic = material.metallicSlot;
    float roughness = material.roughness;
    float ao = material.ao;

    if (material.enableMetallicMap) {
        metallic = texture(material.metallicSlot, uv).r;
    }

    if (material.enableRoughnessMap) {
        roughness = texture(material.roughnessSlot, uv).r;
    }

    if (material.enableAOMap) {
        ao = texture(material.aoSlot, uv).r;
    }

    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance    = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lightColors[i] * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);

    vec3 color = amb * (albedo + diff + specular);
    return vec4(gamma(color.rgb, material.gamma), material.color.w);
}