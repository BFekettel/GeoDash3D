//
// Created by Ben on 14/08/2025.
//

#include "StaticMeshComp.h"
#include "../Entity/Entity.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Developer/globals.h"


StaticMeshComp::StaticMeshComp() {
    loadModel("../resources/models/cube.obj");
}


StaticMeshComp::~StaticMeshComp() { //deletes buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void StaticMeshComp::loadModel(const char *path) {
        // MAKE LOAD MODEL FUNCTION
    // Setup VAO/VBO/EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // Model Loading
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // For now, just grab the first mesh
    aiMesh* mesh = scene->mMeshes[0];


    /*
     Current Array Setup, the order we place information into the
     verticies array matter in terms of how the shader interprates the info
        0,1,2
        Location X Y Z

        3,4,5
        Color RGB

        6,7,8
        Normal X Y Z

     */


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
            vertices.push_back(i % 2);
            vertices.push_back(i % 2);
            vertices.push_back(i % 2);
        }

        // Normals
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        } else {
            vertices.push_back(1.0);
            vertices.push_back(1.0);
            vertices.push_back(1.0);
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

    // Attributes: pos(3) + color(3) + normal(3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    indexCount = static_cast<unsigned int>(indices.size());
}

void StaticMeshComp::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    if (visible) {
        shader.use();
        //Passing Uniform into shader program
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setFloat("ambientStrength", GlobalAmbientStrength);
        shader.setVec3("lightColor", GlobalLightColor); //Need to get from light source object
        shader.setVec3("lightPos", GlobalLightPos); //Need to make light source object


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); //Draws model from shader results
    }
}
