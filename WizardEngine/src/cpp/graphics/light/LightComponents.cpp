//
// Created by mecha on 06.06.2022.
//

#include <graphics/light/LightComponents.h>

namespace engine::graphics {

    ShaderScript phongLightScript() {
        auto script = ShaderScript();

        script.updateRegistry = [](const BaseShaderProgram& shader, ecs::Registry& registry) {
            u32 i = 0;
            registry.each<PhongLightComponent>([&shader, &i](PhongLightComponent* phongLight) {
                shader.setUniformArrayStructField(i, phongLight->name, phongLight->position);
                shader.setUniformArrayStructField(i, phongLight->name, phongLight->color);
                shader.setUniformArrayStructField(i, phongLight->name, phongLight->ambient);
                shader.setUniformArrayStructField(i, phongLight->name, phongLight->diffuse);
                shader.setUniformArrayStructField(i, phongLight->name, phongLight->specular);
                i++;
            });
        };

        script.updateEntity = [](const BaseShaderProgram& shader, const ecs::Entity& entity) {
            auto phongLight = entity.get<PhongLightComponent>();
            if (phongLight) {
                shader.setUniformStructField(phongLight->name, phongLight->position);
                shader.setUniformStructField(phongLight->name, phongLight->color);
                shader.setUniformStructField(phongLight->name, phongLight->ambient);
                shader.setUniformStructField(phongLight->name, phongLight->diffuse);
                shader.setUniformStructField(phongLight->name, phongLight->specular);
            }
        };

        return script;
    }

    void PhongLightComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "PhongLightComponent";
        yaml::serialize(out, "name", name);
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "color", color);
        yaml::serialize(out, "ambient", ambient);
        yaml::serialize(out, "diffuse", diffuse);
        yaml::serialize(out, "specular", specular);
        out << YAML::EndMap;
    }

    void PhongLightComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["PhongLightComponent"];
        if (root) {
            yaml::deserialize(root, "name", name);
            yaml::deserialize(root, "position", position);
            yaml::deserialize(root, "color", color);
            yaml::deserialize(root, "ambient", ambient);
            yaml::deserialize(root, "diffuse", diffuse);
            yaml::deserialize(root, "specular", specular);
        }
    }

    void DirectLightComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "DirectLightComponent";
        yaml::serialize(out, "name", name);
        yaml::serialize(out, "direction", direction);
        yaml::serialize(out, "ambient", ambient);
        yaml::serialize(out, "diffuse", diffuse);
        yaml::serialize(out, "specular", specular);
        out << YAML::EndMap;
    }

    void DirectLightComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["DirectLightComponent"];
        if (root) {
            yaml::deserialize(root, "name", name);
            yaml::deserialize(root, "direction", direction);
            yaml::deserialize(root, "ambient", ambient);
            yaml::deserialize(root, "diffuse", diffuse);
            yaml::deserialize(root, "specular", specular);
        }
    }

    void PointLightComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "PointLightComponent";
        yaml::serialize(out, "name", name);
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "ambient", ambient);
        yaml::serialize(out, "diffuse", diffuse);
        yaml::serialize(out, "specular", specular);
        yaml::serialize(out, "constant", constant);
        yaml::serialize(out, "linear", linear);
        yaml::serialize(out, "quadratic", quadratic);
        out << YAML::EndMap;
    }

    void PointLightComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["PointLightComponent"];
        if (root) {
            yaml::deserialize(root, "name", name);
            yaml::deserialize(root, "position", position);
            yaml::deserialize(root, "ambient", ambient);
            yaml::deserialize(root, "diffuse", diffuse);
            yaml::deserialize(root, "specular", specular);
            yaml::deserialize(root, "constant", constant);
            yaml::deserialize(root, "linear", linear);
            yaml::deserialize(root, "quadratic", quadratic);
        }
    }

    void FlashLightComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "FlashLightComponent";
        yaml::serialize(out, "name", name);
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "direction", direction);
        yaml::serialize(out, "cutoff", cutoff);
        yaml::serialize(out, "outerCutoff", outerCutoff);
        yaml::serialize(out, "ambient", ambient);
        yaml::serialize(out, "diffuse", diffuse);
        yaml::serialize(out, "specular", specular);
        yaml::serialize(out, "constant", constant);
        yaml::serialize(out, "linear", linear);
        yaml::serialize(out, "quadratic", quadratic);
        out << YAML::EndMap;
    }

    void FlashLightComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["FlashLightComponent"];
        if (root) {
            yaml::deserialize(root, "name", name);
            yaml::deserialize(root, "position", position);
            yaml::deserialize(root, "direction", direction);
            yaml::deserialize(root, "cutoff", cutoff);
            yaml::deserialize(root, "outerCutoff", outerCutoff);
            yaml::deserialize(root, "ambient", ambient);
            yaml::deserialize(root, "diffuse", diffuse);
            yaml::deserialize(root, "specular", specular);
            yaml::deserialize(root, "constant", constant);
            yaml::deserialize(root, "linear", linear);
            yaml::deserialize(root, "quadratic", quadratic);
        }
    }

}