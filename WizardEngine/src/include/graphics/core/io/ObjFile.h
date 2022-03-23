//
// Created by mecha on 29.09.2021.
//

#pragma once

#include <core/FileSystem.h>
#include <graphics/core/geometry/Mesh.h>

#define RUNTIME_OBJ_PATH "assets/obj"

namespace engine {

    struct Face {
        int posIndex;
        int uvIndex;
        int normalIndex;
    };

    struct ObjVertex {
        glm::vec3 position = { 0.5f, 0.5f, 0.5f };
        glm::vec2 textureCoords = { 0.25f, -0.25f };
        glm::vec3 normal = { 0, 0, 0 };
    };

    typedef BaseMesh<ObjVertex> ObjMesh;
    typedef BaseMeshComponent<ObjVertex> ObjMeshComponent;

    class ObjFile final {

    private:
        ObjFile() = default;
        ~ObjFile() = default;

    public:
        static ObjMeshComponent read(const std::string &fileName);

    private:
        static void createMesh();
        static void clearVertexCache();
        static void clearMeshCache();
        static void resetVertexCounters();

    private:
        static std::vector<ObjVertex> vertices;
        static std::vector<glm::vec2> uvs;
        static std::vector<glm::vec3> normals;
        static std::vector<uint32_t> indices;
        static std::vector<Face> faces;
        static std::vector<ObjMesh> meshes;

        static uint32_t vertexCounter;
        static uint32_t indexCounter;
        static uint32_t uvsCounter;
        static uint32_t normalsCounter;

        static std::string_view _fileName;
    };

    template<typename T>
    T mapPos(const ObjVertex& objVertex) {
        return { objVertex.position };
    }

    template<typename T>
    T mapPosUv(const ObjVertex& objVertex) {
        return { objVertex.position, objVertex.textureCoords };
    }

    template<typename T>
    T map(const ObjVertex& objVertex) {
        return { objVertex.position, objVertex.textureCoords, objVertex.normal };
    }
}
