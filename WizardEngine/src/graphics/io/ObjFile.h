//
// Created by mecha on 29.09.2021.
//

#pragma once

#include "../../core/String.h"
#include "../../core/File.h"

#include "../buffers/VertexData.h"
#include "../buffers/IndexData.h"

#include "vector"

#define OBJ_PATH "assets/obj/"

namespace engine {

    struct ObjModel {
        VertexData vertexData;
        IndexData indexData;
    };

    class ObjFile : File {

    public:
        ObjFile() = default;
        ObjFile(const std::string& name, const std::string &path) : File(name, path) {}
        ~ObjFile() override = default;

    public:
        ObjModel read() const;
        ObjModel read(const std::string &fileName) const;

    protected:
        const char *getExtensionName() const override;
        const char *getAssetPath() const override;

    private:
        std::vector<Vertex> vertices;


    };

}