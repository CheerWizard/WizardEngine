//
// Created by mecha on 29.09.2021.
//

#pragma once

#include <core/FileSystem.h>
#include <graphics/core/geometry/MeshComponent.h>

#define RUNTIME_OBJ_PATH "assets/obj"

namespace engine {

    struct Face {
        int posIndex;
        int uvIndex;
        int normalIndex;
    };

    class ObjFile final {

    private:
        ObjFile() = default;
        ~ObjFile() = default;

    public:
        static MeshComponent read(const std::string &fileName);

    private:
        static void createMesh();
        static void clearVertexCache();
        static void clearMeshCache();
        static void resetVertexCounters();

    private:
        static std::vector<Vertex> vertices;
        static std::vector<glm::vec2> uvs;
        static std::vector<glm::vec3> normals;
        static std::vector<uint32_t> indices;
        static std::vector<Face> faces;
        static std::vector<Mesh> meshes;

        static uint32_t vertexCounter;
        static uint32_t indexCounter;
        static uint32_t uvsCounter;
        static uint32_t normalsCounter;

        static std::string_view _fileName;
    };

}