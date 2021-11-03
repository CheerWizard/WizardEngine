//
// Created by mecha on 29.09.2021.
//

#pragma once

#include "../../../../core/File.h"

#include "../../buffers/VertexData.h"
#include "../../buffers/IndexData.h"

#include "../../geometry/MeshComponent.h"

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
        ~ObjFile() = default;

    public:
        MeshComponent readObj(const std::string &fileName);

    protected:
        const char *getExtensionName() const override;

    private:
        void createMesh();
        void clearVertexCache();
        void clearMeshCache();
        void resetVertexCounters();

    private:
        std::vector<Vertex> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<uint32_t> indices;
        std::vector<Face> faces;
        std::vector<Mesh> meshes;

        uint32_t vertexCounter = 0;
        uint32_t indexCounter = 0;
        uint32_t uvsCounter = 0;
        uint32_t normalsCounter = 0;
    };

}