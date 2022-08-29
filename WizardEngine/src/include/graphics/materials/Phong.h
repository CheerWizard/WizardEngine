//
// Created by mecha on 24.08.2022.
//

#pragma once

#include <graphics/materials/Material.h>

namespace engine::graphics {

    struct Phong : Material {

        Phong(const char* name = "phong") : Material(name) {
            vec4.emplace_back("color", vec4f { 0.5, 0.5, 0.5, 1 });

            floats.emplace_back("ambient", 1);
            floats.emplace_back("diffuse", 1);
            floats.emplace_back("specular", 1);
            floats.emplace_back("shiny", 1);
            floats.emplace_back("gamma", 2.2);
            floats.emplace_back("heightScale", 0.1);
            floats.emplace_back("minLayers", 8);
            floats.emplace_back("maxLayers", 32);

            textures.emplace_back(IntUniform { "albedo", 0 });
            textures.emplace_back(IntUniform { "diffuseMap", 1 });
            textures.emplace_back(IntUniform { "specularMap", 2 });
            textures.emplace_back(IntUniform { "normalMap", 3 });
            textures.emplace_back(IntUniform { "depthMap", 4 });

            flags.emplace_back("enableAlbedoMap", false);
            flags.emplace_back("enableDiffuseMap", false);
            flags.emplace_back("enableSpecularMap", false);
            flags.emplace_back("enableBlinn", false);
            flags.emplace_back("enableNormalMap", false);
            flags.emplace_back("enableParallaxMap", false);
        }

        Phong(const vec4f& colorValue) : Material("phong") {
            vec4.emplace_back("color", colorValue);

            floats.emplace_back("ambient", 1);
            floats.emplace_back("diffuse", 1);
            floats.emplace_back("specular", 1);
            floats.emplace_back("shiny", 1);
            floats.emplace_back("gamma", 2.2);
            floats.emplace_back("heightScale", 0.1);
            floats.emplace_back("minLayers", 8);
            floats.emplace_back("maxLayers", 32);

            textures.emplace_back(IntUniform { "albedo", 0 });
            textures.emplace_back(IntUniform { "diffuseMap", 1 });
            textures.emplace_back(IntUniform { "specularMap", 2 });
            textures.emplace_back(IntUniform { "normalMap", 3 });
            textures.emplace_back(IntUniform { "depthMap", 4 });

            flags.emplace_back("enableAlbedoMap", false);
            flags.emplace_back("enableDiffuseMap", false);
            flags.emplace_back("enableSpecularMap", false);
            flags.emplace_back("enableBlinn", false);
            flags.emplace_back("enableNormalMap", false);
            flags.emplace_back("enableParallaxMap", false);
        }

        inline Vec4fUniform& color() {
            return vec4[0];
        }

        inline FloatUniform& ambient() {
            return floats[0];
        }

        inline FloatUniform& diffuse() {
            return floats[1];
        }

        inline FloatUniform& specular() {
            return floats[2];
        }

        inline FloatUniform& shiny() {
            return floats[3];
        }

        inline FloatUniform& gamma() {
            return floats[4];
        }

        inline FloatUniform& heightScale() {
            return floats[5];
        }

        inline FloatUniform& minLayers() {
            return floats[6];
        }

        inline FloatUniform& maxLayers() {
            return floats[7];
        }

        inline TextureComponent& albedo() {
            return textures[0];
        }

        inline TextureComponent& diffuseMap() {
            return textures[1];
        }

        inline TextureComponent& specularMap() {
            return textures[2];
        }

        inline TextureComponent& normalMap() {
            return textures[3];
        }

        inline TextureComponent& depthMap() {
            return textures[4];
        }

        inline BoolUniform& enableAlbedoMap() {
            return flags[0];
        }

        inline BoolUniform& enableDiffuseMap() {
            return flags[1];
        }

        inline BoolUniform& enableSpecularMap() {
            return flags[2];
        }

        inline BoolUniform& enableBlinn() {
            return flags[3];
        }

        inline BoolUniform& enableNormalMap() {
            return flags[4];
        }

        inline BoolUniform& enableParallaxMap() {
            return flags[5];
        }
    };

}