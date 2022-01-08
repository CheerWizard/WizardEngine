//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "cstdint"

namespace engine {
    void drawQuads(const uint32_t &indexCount);
    void drawTriangles(const uint32_t &indexCount);
    void drawTriangles(const uint32_t &indexCount, const uint32_t &instanceCount);
    void enableCulling();
    void disableCulling();
    void setPolygonMode(const uint8_t &mode);
    const unsigned char* getAPIName();
    const unsigned char* getVendorName();
    const unsigned char* getRendererName();
    const unsigned char* getVersion();
    void enableMSAA();
    void disableMSAA();
}
