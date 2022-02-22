//
// Created by mecha on 29.09.2021.
//

#include <graphics/core/io/ObjFile.h>
#include "sstream"

namespace engine {

    uint32_t ObjFile::vertexCounter = 0;
    uint32_t ObjFile::normalsCounter = 0;
    uint32_t ObjFile::uvsCounter = 0;
    uint32_t ObjFile::indexCounter = 0;
    std::vector<Vertex> ObjFile::vertices = {};
    std::vector<glm::vec2> ObjFile::uvs = {};
    std::vector<glm::vec3> ObjFile::normals = {};
    std::vector<uint32_t> ObjFile::indices = {};
    std::vector<Face> ObjFile::faces = {};
    std::vector<Mesh> ObjFile::meshes = {};
    std::string_view ObjFile::_fileName;

    MeshComponent ObjFile::read(const std::string &fileName) {
        _fileName = fileName;
        // read source from full path
        std::stringstream fullPath;
        fullPath << RUNTIME_OBJ_PATH << "/" << fileName;
        auto source = FileSystem::read(fullPath.str());
        // tokenize source
        auto tokens = split(source, "\n\r ");
        // find tokens and convert it into vertices, indices and faces!
        bool faceFound = false;
        for (uint32_t i = 0 ; i < tokens.size(); i++) {
            auto token = tokens[i];

            if (token == "v") {
                // check if we found a face previously. if yes, then create a mesh and continue parsing
                if (faceFound) {
                    faceFound = false;
                    createMesh();
                }

                glm::vec3 position = {
                        TO_FLOAT(tokens[i + 1]),
                        TO_FLOAT(tokens[i + 2]),
                        TO_FLOAT(tokens[i + 3])
                };
                vertices.emplace_back(Vertex { position });

            } else if (token == "vt") {
                glm::vec2 uv = {
                        TO_FLOAT(tokens[i + 1]),
                        TO_FLOAT(tokens[i + 2])
                };
                uvs.emplace_back(uv);

            } else if (token == "vn") {
                glm::vec3 normal = {
                        TO_FLOAT(tokens[i + 1]),
                        TO_FLOAT(tokens[i + 2]),
                        TO_FLOAT(tokens[i + 3])
                };
                normals.emplace_back(normal);

            } else if (token == "f") {
                faceFound = true;
                auto vertexOrder1Token = tokens[i + 1];
                auto vertexOrder2Token = tokens[i + 2];
                auto vertexOrder3Token = tokens[i + 3];

                auto faceTokens1 = split(vertexOrder1Token, "/");
                auto faceTokens2 = split(vertexOrder2Token, "/");
                auto faceTokens3 = split(vertexOrder3Token, "/");

                int indexUv1 = 0;
                int indexPos1;
                int indexNormal1;
                if (faceTokens1.size() < 3) {
                    indexPos1 = TO_INT(faceTokens1[0]) - 1;
                    indexNormal1 = TO_INT(faceTokens1[1]) - 1;
                } else {
                    indexPos1 = TO_INT(faceTokens1[0]) - 1;
                    indexUv1 = TO_INT(faceTokens1[1]) - 1;
                    indexNormal1 = TO_INT(faceTokens1[2]) - 1;
                }

                Face face1 = {
                        indexPos1,
                        indexUv1,
                        indexNormal1
                };

                int indexUv2 = 0;
                int indexPos2 = 0;
                int indexNormal2;
                if (faceTokens2.size() < 3) {
                    indexPos2 = TO_INT(faceTokens2[0]) - 1;
                    indexNormal2 = TO_INT(faceTokens2[1]) - 1;
                } else {
                    indexPos2 = TO_INT(faceTokens2[0]) - 1;
                    indexUv2 = TO_INT(faceTokens2[1]) - 1;
                    indexNormal2 = TO_INT(faceTokens2[2]) - 1;
                }

                Face face2 = {
                        indexPos2,
                        indexUv2,
                        indexNormal2
                };

                int indexUv3 = 0;
                int indexPos3 = 0;
                int indexNormal3;
                if (faceTokens3.size() < 3) {
                    indexPos3 = TO_INT(faceTokens3[0]) - 1;
                    indexNormal3 = TO_INT(faceTokens3[1]) - 1;
                } else {
                    indexPos3 = TO_INT(faceTokens3[0]) - 1;
                    indexUv3 = TO_INT(faceTokens3[1]) - 1;
                    indexNormal3 = TO_INT(faceTokens3[2]) - 1;
                }

                Face face3 = {
                        indexPos3,
                        indexUv3,
                        indexNormal3
                };

                indices.emplace_back(indexPos1);
                indices.emplace_back(indexPos2);
                indices.emplace_back(indexPos3);

                faces.emplace_back(face1);
                faces.emplace_back(face2);
                faces.emplace_back(face3);

                if (tokens.size() == i + 4) break;

                auto vertexOrder4Token = tokens[i + 4];
                auto faceTokens4 = split(vertexOrder4Token, "/");

                // todo currently we are handling 4 faces. need to find a way to handle any count of faces!
                if (faceTokens4.size() > 1) {
                    int indexUv4 = 0;
                    int indexPos4;
                    int indexNormal4;
                    if (faceTokens4.size() < 3) {
                        indexPos4 = TO_INT(faceTokens4[0]) - 1;
                        indexNormal4 = TO_INT(faceTokens4[1]) - 1;
                    } else {
                        indexPos4 = TO_INT(faceTokens4[0]) - 1;
                        indexUv4 = TO_INT(faceTokens4[1]) - 1;
                        indexNormal4 = TO_INT(faceTokens4[2]) - 1;
                    }

                    Face face4 = {
                        indexPos4,
                        indexUv4,
                        indexNormal4
                    };

                    indices.emplace_back(indexPos1);
                    indices.emplace_back(indexPos3);
                    indices.emplace_back(indexPos4);

                    faces.emplace_back(face4);
                }
            }
        }

        // creates last mesh
        if (faceFound) {
            createMesh();
        }

        // convert meshes vector into meshes array
        uint32_t meshCount = meshes.size();
        auto* meshArr = new Mesh[meshCount];
        for (uint32_t i = 0; i < meshCount ; i++) {
            meshArr[i] = meshes[i];
        }
        clearMeshCache();
        resetVertexCounters();

        return { meshArr, meshCount };
    }

    void ObjFile::createMesh() {
        if (uvs.empty()) {
            ENGINE_WARN("ObjFile: No UVs for {0} file!", _fileName);
        }

        if (normals.empty()) {
            ENGINE_WARN("ObjFile: No normals for {0} file!", _fileName);
        }

        for (const auto& face : faces) {
            auto posIndex = face.posIndex - vertexCounter;
            auto uvIndex = face.uvIndex - uvsCounter;
            auto normalIndex = face.normalIndex - normalsCounter;

            auto& vertex = vertices[posIndex];
            if (!uvs.empty()) {
                vertex.textureCoords = uvs[uvIndex];
            }
            if (!normals.empty()) {
                vertex.normal = normals[normalIndex];
            }
        }

        uint32_t indexCount = indices.size();
        uint32_t vertexCount = vertices.size();
        auto* indicesArr = new uint32_t[indexCount];
        auto* verticesArr = new Vertex[vertexCount];

        for (auto j = 0 ; j < indexCount ; j++) {
            indicesArr[j] = indices[j];
        }

        for (auto j = 0 ; j < vertexCount ; j++) {
            verticesArr[j] = vertices[j];
        }

        auto vertexData = VertexData {
            verticesArr,
            vertexCounter,
            vertexCount
        };

        auto indexData = IndexData {
            indicesArr,
            indexCounter,
            indexCount
        };

        meshes.emplace_back(Mesh {
            vertexData,
            indexData
        });

        // invalidate counters
        vertexCounter += vertices.size();
        uvsCounter += uvs.size();
        normalsCounter += normals.size();
        indexCounter += indices.size();

        clearVertexCache();
    }

    void ObjFile::clearVertexCache() {
        vertices.clear();
        uvs.clear();
        normals.clear();
        indices.clear();
        faces.clear();
    }

    void ObjFile::clearMeshCache() {
        meshes.clear();
    }

    void ObjFile::resetVertexCounters() {
        vertexCounter = 0;
        uvsCounter = 0;
        normalsCounter = 0;
        indexCounter = 0;
    }
}