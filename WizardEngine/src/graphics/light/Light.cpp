//
// Created by mecha on 04.11.2021.
//

#include "Light.h"

namespace engine {

    void RegularLight::create() {
        add<AmbientLightComponent>(LightComponents::newAmbient());
        add<DiffuseLightComponent>(LightComponents::newDiffuse());
        add<SpecularLightComponent>(LightComponents::newSpecular());
    }

}