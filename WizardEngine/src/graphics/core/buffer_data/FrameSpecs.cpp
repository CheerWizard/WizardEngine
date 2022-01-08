//
// Created by mecha on 12.11.2021.
//

#include "FrameSpecs.h"

#include "sstream"

namespace engine {

    std::string str(const Color &color) {
        std::stringstream ss;
        ss << "r: " << color.r << " g: " << color.g << " b: " << color.b << " a: " << color.a;
        return ss.str();
    }

}