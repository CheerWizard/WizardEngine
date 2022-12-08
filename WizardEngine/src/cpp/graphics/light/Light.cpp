//
// Created by mecha on 04.11.2021.
//

#include <graphics/light/Light.h>

namespace engine::graphics {

    void Light::create() {
        add<LightComponent>(LightComponent());
    }

    vec3f& Light::getPosition() {
        return get<LightComponent>()->position.value;
    }

    vec4f &Light::getColor() {
        return get<LightComponent>()->color.value;
    }

    void PhongLight::create() {
        add<PhongLightComponent>(PhongLightComponent());
    }

    vec3f& PhongLight::getPosition() {
        return get<PhongLightComponent>()->position.value;
    }

    vec4f &PhongLight::getColor() {
        return get<PhongLightComponent>()->color.value;
    }

    void DirectLight::create() {
        add<DirectLightComponent>(DirectLightComponent());
    }

    void PointLight::create() {
        add<PointLightComponent>(PointLightComponent());
    }

    void FlashLight::create() {
        add<FlashLightComponent>(FlashLightComponent());
    }

}