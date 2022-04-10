//
// Created by mecha on 29.09.2021.
//

#include <core/Mappers.h>
#include <graphics/core/io/ModelFile.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <sstream>

namespace engine::io {

    Model ModelFile::read(const std::string &filePath) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            ENGINE_ERR("Assimp: failed to import file '{0}'; error: {1}", filePath, import.GetErrorString());
            return {};
        }

        std::vector<ModelMesh> meshes;
        std::vector<ModelMaterial> materials;
        extractNodes(scene->mRootNode, scene, meshes, materials);
        graphics::BaseMesh<ModelVertex>* modelMeshes = engine::core::map(meshes);
        ModelMeshComponent meshComponent = { modelMeshes };
        meshComponent.meshCount = meshes.size();

        return { meshComponent, materials };
    }

    void ModelFile::extractNodes(
            aiNode *node,
            const aiScene *scene,
            std::vector<ModelMesh>& meshes,
            std::vector<ModelMaterial>& materials
    ) {
        // extract all meshes and materials of node
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(extractMesh(mesh));
            materials.push_back(extractMaterial(mesh, scene));
        }
        // recursively extract node's children
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            extractNodes(node->mChildren[i], scene, meshes, materials);
        }
    }

    ModelMesh ModelFile::extractMesh(aiMesh *mesh) {
        auto* vertices = new ModelVertex[mesh->mNumVertices];
        std::vector<uint32_t> indicesVector;
        // extract vertices
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            vertices[i].position = {
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z,
            };
            vertices[i].normal = {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z,
            };
            // mesh may not always contain texture coords
            if (mesh->mTextureCoords[0]) {
                vertices[i].uv = {
                        mesh->mTextureCoords[0][i].x,
                        mesh->mTextureCoords[0][i].y
                };
            }
        }
        // extract indices
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            auto face = mesh->mFaces[i];
            for(uint32_t j = 0; j < face.mNumIndices; j++) {
                indicesVector.push_back(face.mIndices[j]);
            }
        }

        graphics::VertexData<ModelVertex> vertexData = { vertices, 0, mesh->mNumVertices };
        graphics::IndexData indexData = {
                engine::core::map(indicesVector),
                0,
                static_cast<uint32_t>(indicesVector.size())
        };

        return { vertexData, indexData };
    }

    ModelMaterial ModelFile::extractMaterial(aiMesh *mesh, const aiScene* scene) {
        std::vector<ModelTexture> textures;
        // check if material exist for mesh
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            extractTextures(textures, material, aiTextureType_DIFFUSE);
            extractTextures(textures, material, aiTextureType_SPECULAR);
            extractTextures(textures, material, aiTextureType_SPECULAR);
        }
        return { textures };
    }

    void ModelFile::extractTextures(
            std::vector<ModelTexture>& textures,
            aiMaterial *mat,
            const aiTextureType& type
    ) {
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString filePath;
            mat->GetTexture(type, i, &filePath);
            ModelTexture texture {
                0,
                type,
                std::string(filePath.data)
            };
            textures.push_back(texture);
        }
    }
}