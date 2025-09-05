//
// Created by Ben on 14/08/2025.
//

#include "StaticShapeComp.h"
#include "../Entity/Entity.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstdlib>
#include <ctime>

StaticShapeComp::StaticShapeComp() {
    // Setup VAO/VBO/EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // Model Loading
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("../resources/models/monkey.obj",
        aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // For now, just grab the first mesh
    aiMesh* mesh = scene->mMeshes[0];

    // Extract vertex data
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // Position
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // Color (fallback: white if no vertex colors)
        if (mesh->HasVertexColors(0)) {
            vertices.push_back(mesh->mColors[0][i].r);
            vertices.push_back(mesh->mColors[0][i].g);
            vertices.push_back(mesh->mColors[0][i].b);
        } else {
            vertices.push_back(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
            vertices.push_back(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
            vertices.push_back(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
        }
    }
    // Extract indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Upload to GPU
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Attributes: pos(3) + color(3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    indexCount = static_cast<unsigned int>(indices.size());
}


StaticShapeComp::~StaticShapeComp() { //deletes buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void StaticShapeComp::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    //Passing Uniform into shader program
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); //Draws model from shader results
}
