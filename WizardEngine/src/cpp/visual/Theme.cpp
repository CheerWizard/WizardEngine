//
// Created by mecha on 01.09.2022.
//

#include <visual/Theme.h>
#include <math/Math.h>

namespace engine::visual {

    int Theme::accent					= IM_COL32(236, 158, 36, 255);
    int Theme::highlight				= IM_COL32(39, 185, 242, 255);
    int Theme::niceBlue					= IM_COL32(83, 232, 254, 255);
    int Theme::compliment				= IM_COL32(78, 151, 166, 255);
    int Theme::background				= IM_COL32(36, 36, 36, 255);
    int Theme::backgroundDark			= IM_COL32(26, 26, 26, 255);
    int Theme::titlebar					= IM_COL32(21, 21, 21, 255);
    int Theme::propertyField			= IM_COL32(15, 15, 15, 255);
    int Theme::text						= IM_COL32(192, 192, 192, 255);
    int Theme::textBrighter				= IM_COL32(210, 210, 210, 255);
    int Theme::textDarker				= IM_COL32(128, 128, 128, 255);
    int Theme::muted					= IM_COL32(77, 77, 77, 255);
    int Theme::groupHeader				= IM_COL32(47, 47, 47, 255);
//        int Theme::selection				= IM_COL32(191, 177, 155, 255);
//        int Theme::selectionMuted			= IM_COL32(59, 57, 45, 255);
    int Theme::selection				= IM_COL32(237, 192, 119, 255);
    int Theme::selectionMuted			= IM_COL32(237, 201, 142, 23);
//        int Theme::backgroundPopup			= IM_COL32(63, 73, 77, 255); // in between
//        int Theme::backgroundPopup			= IM_COL32(63, 77, 76, 255); // most green
    int Theme::backgroundPopup			= IM_COL32(63, 70, 77, 255); // most blue

    int Theme::button                 = IM_COL32(0.1, 0.6, 0.8, 1);
    int Theme::buttonHover                 = IM_COL32(0.2, 0.7, 0.9, 1);
    int Theme::buttonPressed                 = IM_COL32(0.1, 0.6, 0.8, 1);

    float Theme::Convert_sRGB_FromLinear(float theLinearValue) {
        return theLinearValue <= 0.0031308f
               ? theLinearValue * 12.92f
               : powf(theLinearValue, 1.0f / 2.2f) * 1.055f - 0.055f;
    }

    float Theme::Convert_sRGB_ToLinear(float thesRGBValue) {
        return thesRGBValue <= 0.04045f
               ? thesRGBValue / 12.92f
               : powf((thesRGBValue + 0.055f) / 1.055f, 2.2f);
    }

    ImVec4 Theme::ConvertFromSRGB(ImVec4 colour) {
        return {
                Convert_sRGB_FromLinear(colour.x),
                Convert_sRGB_FromLinear(colour.y),
                Convert_sRGB_FromLinear(colour.z),
                colour.w
        };
    }

    ImVec4 Theme::ConvertToSRGB(ImVec4 colour) {
        return {
                std::pow(colour.x, 2.2f),
                std::pow(colour.y, 2.2f),
                std::pow(colour.z, 2.2f),
                colour.w
        };
    }

}