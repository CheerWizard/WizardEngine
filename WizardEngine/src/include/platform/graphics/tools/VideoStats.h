//
// Created by mecha on 29.08.2022.
//

#pragma once

namespace engine::graphics {

    class VideoStats final {
    public:
        static int getMaxSlots();
        static const char* getAPIName();
        static const unsigned char * getVendorName();
        static const unsigned char* getRendererName();
        static const unsigned char* getVersion();
    };

}
