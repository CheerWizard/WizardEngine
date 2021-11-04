//
// Created by mecha on 04.11.2021.
//

#include "Light.h"

namespace engine {

    void RegularLight::create() {
        add<AmbientLightComponent>(AmbientLightComponent());
        add<DiffuseLightComponent>(DiffuseLightComponent());
        add<SpecularLightComponent>(SpecularLightComponent());
    }

}