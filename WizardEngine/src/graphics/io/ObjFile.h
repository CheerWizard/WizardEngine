//
// Created by mecha on 29.09.2021.
//

#pragma once

#include "../../core/File.h"

#include "../buffers/VertexData.h"
#include "../buffers/IndexData.h"

#define OBJ_PATH "obj"

namespace engine {

    struct ObjData {
        VertexData vertexData;
        IndexData indexData;
    };

    struct Face {
        int posIndex;
        int uvIndex;
        int normalIndex;
    };

    class ObjFile : public File {

    public:
        ObjFile() = default;
        ObjFile(const std::string& name, const std::string &path) : File(name, path) {}

        ~ObjFile() override {
            destroy();
        }

    public:
        ObjData readObj(const std::string &fileName);

    protected:
        const char *getExtensionName() const override;
        const char *getAssetPath() const override;

    private:
        void destroy();

    };

}