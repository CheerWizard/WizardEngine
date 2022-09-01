//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <imgui.h>

namespace engine::visual {

    class Theme final {

    public:
        float Convert_sRGB_FromLinear(float theLinearValue);
        float Convert_sRGB_ToLinear(float thesRGBValue);
        ImVec4 ConvertFromSRGB(ImVec4 colour);
        ImVec4 ConvertToSRGB(ImVec4 colour);

    public:
        static int accent;
        static int highlight;
        static int niceBlue;
        static int compliment;
        static int background;
        static int backgroundDark;
        static int titlebar;
        static int propertyField;
        static int text;
        static int textBrighter;
        static int textDarker;
        static int muted;
        static int groupHeader;
        static int selection;
        static int selectionMuted;
        static int backgroundPopup;
        static int button;
        static int buttonHover;
        static int buttonPressed;
    };

}