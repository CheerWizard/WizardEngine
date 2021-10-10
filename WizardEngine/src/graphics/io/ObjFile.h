//
// Created by mecha on 29.09.2021.
//

#pragma once

#include "../../core/File.h"

#include "../buffers/VertexData.h"
#include "../buffers/IndexData.h"

#include "../geometry/MeshComponent.h"

#include "unordered_map"

#define OBJ_PATH "assets/obj"

namespace engine {

    struct Face {
        int posIndex;
        int uvIndex;
        int normalIndex;
    };

    class ObjFile : public File {

    public:
        ObjFile() : File("", OBJ_PATH) {}

        ~ObjFile() override {
            destroy();
        }

    public:
        Ref<MeshComponent> readObj(const std::string &fileName);

    protected:
        const char *getExtensionName() const override;

    private:
        bool exists(const std::string &objName);
        void destroy();

    private:
        std::unordered_map<std::string, Ref<MeshComponent>> _meshComponents;

    };

}