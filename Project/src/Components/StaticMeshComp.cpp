//
// Created by Ben on 14/08/2025.
//

#include "StaticMeshComp.h"
#include "../Entity/Entity.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Developer/globals.h"

StaticMeshComp::StaticMeshComp() {
    loadModel("../content/models/cube.obj");
}

StaticMeshComp::~StaticMeshComp() { // deletes buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

unsigned int StaticMeshComp::loadTexture(const char* filename) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void StaticMeshComp::loadModel(const char *path) {
    // Setup VAO/VBO/EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Model Loading
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    aiMesh* mesh = scene->mMeshes[0];

    // --- Vertex Data ---
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // Position
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // Color
        if (mesh->HasVertexColors(0)) {
            vertices.push_back(mesh->mColors[0][i].r);
            vertices.push_back(mesh->mColors[0][i].g);
            vertices.push_back(mesh->mColors[0][i].b);
        } else {
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
        }

        // Normal
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        } else {
            vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f});
        }

        // Texture Coordinates
        if (mesh->HasTextureCoords(0)) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // Tangent
        if (mesh->HasTangentsAndBitangents()) {
            vertices.push_back(mesh->mTangents[i].x);
            vertices.push_back(mesh->mTangents[i].y);
            vertices.push_back(mesh->mTangents[i].z);

            vertices.push_back(mesh->mBitangents[i].x);
            vertices.push_back(mesh->mBitangents[i].y);
            vertices.push_back(mesh->mBitangents[i].z);
        } else {
            vertices.insert(vertices.end(), {0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f});
        }
    }

    // --- Load Material ---
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    loadMaterial(material);

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // --- Upload to GPU ---
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Layout: pos(3) + color(3) + normal(3) + texcoord(2) + tangent(3) + bitangent(3) = 17 floats
    GLsizei stride = 17 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                     // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));   // Color
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));   // Normal
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));   // TexCoord
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(11 * sizeof(float)));  // Tangent
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, stride, (void*)(14 * sizeof(float)));  // Bitangent
    glEnableVertexAttribArray(5);

    glBindVertexArray(0);

    indexCount = static_cast<unsigned int>(indices.size());
}

void StaticMeshComp::Draw(const Shader& shader) {
    if (!visible) return;

    shader.use();

    // Pass model matrix
    shader.setMat4("model", model);

    // --- Diffuse ---
    if (textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.diffuse", 0);
    }

    // --- Normal Map ---
    if (normalMapID != 0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMapID);
        shader.setInt("material.normalMap", 1);
    }

    // --- Specular Map ---
    if (specularMapID != 0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularMapID);
        shader.setInt("material.specularMap", 2);
    }

    // Material properties
    shader.setVec3("material.ambient", ambient);
    shader.setFloat("material.shininess", shininess);

    // Draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Unbind textures
    glBindTexture(GL_TEXTURE_2D, 0);
}


void StaticMeshComp::loadMaterial(aiMaterial* material) {
    // --- Diffuse Texture ---
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

        std::string filename = str.C_Str();
        std::string texturePath = "../content/Textures/" + filename;
        textureID = loadTexture(texturePath.c_str());
    } else {
        textureID = loadTexture("../content/Textures/defaultTexture.jpg");
        std::cout << "No diffuse texture found in material, using default\n";
    }

    // --- Normal Map (OBJ uses HEIGHT/BUMP, FBX uses NORMALS) ---
    aiString str;
    if (material->GetTexture(aiTextureType_NORMALS, 0, &str) == AI_SUCCESS ||
        material->GetTexture(aiTextureType_HEIGHT, 0, &str) == AI_SUCCESS) {

        std::string filename = str.C_Str();
        std::string texturePath = "../content/Textures/" + filename;
        normalMapID = loadTexture(texturePath.c_str());
    } else {
        normalMapID = 0;
        std::cout << "No normal map found in material\n";
    }

    // --- Specular Map ---
    if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        aiString str;
        material->GetTexture(aiTextureType_SPECULAR, 0, &str);

        std::string filename = str.C_Str();
        std::string texturePath = "../content/Textures/" + filename;
        specularMapID = loadTexture(texturePath.c_str());
    } else {
        specularMapID = 0;
        std::cout << "No specular map found in material\n";
    }

    // --- Ambient Color ---
    aiColor3D ambientColor;
    if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor))
        ambientColor = aiColor3D(0.1f, 0.1f, 0.1f);
    ambient = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);

    // --- Shininess ---
    float shininessVal;
    if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininessVal))
        shininessVal = 32.0f;
    shininess = shininessVal;
}
