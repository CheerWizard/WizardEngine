//
// Created by mecha on 29.09.2021.
//

#include "ObjFile.h"

namespace engine {

    const char *ObjFile::getExtensionName() const {
        return "";
    }

    // todo extract multiple meshes from single file into single MeshComponent with multiple meshes.
    MeshComponent ObjFile::readObj(const std::string &fileName) {
        auto source = readAsset(fileName);
        auto tokens = split(source, "\n\r ");

        std::vector<Vertex> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<uint32_t> indices;
        std::vector<Face> faces;

        for (auto i = 0 ; i < tokens.size(); i++) {
            auto token = tokens[i];

            if (token == "v") {
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

        for (const auto& face : faces) {
            auto posIndex = face.posIndex;
            auto uvIndex = face.uvIndex;
            auto& vertex = vertices[posIndex];
            if (!uvs.empty()) {
                vertex.textureCoords = uvs[uvIndex];
            }
        }

        uint32_t indexCount = indices.size();
        uint32_t vertexCount = vertices.size();
        auto* indicesArr = new uint32_t[indexCount];
        auto* verticesArr = new Vertex[vertexCount];

        for (auto i = 0 ; i < indexCount ; i++) {
            indicesArr[i] = indices[i];
        }

        for (auto i = 0 ; i < vertexCount ; i++) {
            verticesArr[i] = vertices[i];
        }

        auto vertexData = VertexData {
            verticesArr,
            0,
            vertexCount
        };

        auto indexData = IndexData {
            indicesArr,
            0,
            indexCount
        };

        auto* mesh = new Mesh {
            vertexData,
            indexData
        };

        return { mesh };
    }

}