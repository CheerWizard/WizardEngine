//
// Created by mecha on 29.09.2021.
//

#pragma once

#include <core/filesystem.h>
#include <graphics/core/geometry/Mesh.h>

#include <assimp/scene.h>

namespace engine::io {

    struct Face {
        int posIndex;
        int uvIndex;
        int normalIndex;
    };

    struct ModelVertex {
        glm::vec3 position = { 0.5f, 0.5f, 0.5f };
        glm::vec2 uv = { 0, 0 };
        glm::vec3 normal = { 0, 0, 0 };
    };

    struct ModelTexture {
        uint32_t id;
        aiTextureType type;
        std::string path;
    };

    struct ModelMaterial {
        std::vector<ModelTexture> textures;
    };

    typedef graphics::BaseMesh<ModelVertex> ModelMesh;
    typedef graphics::BaseMeshComponent<ModelVertex> ModelMeshComponent;

    struct Model {
        ModelMeshComponent meshComponent;
        std::vector<ModelMaterial> materials;
    };

    // const container of aiTextureType and actual const string naming
    class ModelTextureTypes final {

    private:
        ModelTextureTypes() = default;

    public:
        static const char* get(const aiTextureType& aiTextureType) {
            static ModelTextureTypes instance;
            return instance.textureTypes.at(aiTextureType);
        }

    public:
        const std::unordered_map<aiTextureType, const char*> textureTypes = {
                { aiTextureType::aiTextureType_AMBIENT, "ambient" },
                { aiTextureType::aiTextureType_DIFFUSE, "diffuse" },
                { aiTextureType::aiTextureType_SPECULAR, "specular" },
                { aiTextureType::aiTextureType_NORMALS, "normal" },
        };
    };

    class ModelFile final {

    private:
        ModelFile() = default;
        ~ModelFile() = default;

    public:
        static Model read(const std::string &filePath);

    private:
        static void extractNodes(
                aiNode *node,
                const aiScene *scene,
                std::vector<ModelMesh>& meshes,
                std::vector<ModelMaterial>& materials
        );
        static ModelMesh extractMesh(aiMesh *mesh);
        static ModelMaterial extractMaterial(aiMesh *mesh, const aiScene *scene);
        static void extractTextures(std::vector<ModelTexture>& textures, aiMaterial *mat, const aiTextureType& type);
    };

    template<typename T>
    T mapPos(const ModelVertex& modelVertex) {
        return { modelVertex.position };
    }

    template<typename T>
    T mapPosUv(const ModelVertex& modelVertex) {
        return { modelVertex.position, modelVertex.uv };
    }

    template<typename T>
    T map(const ModelVertex& modelVertex) {
        return { modelVertex.position, modelVertex.uv, modelVertex.normal };
    }
}
