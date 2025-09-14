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


StaticMeshComp::~StaticMeshComp() { //deletes buffers
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
        aiProcess_Triangulate | aiProcess_FlipUVs);

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
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // Texture Coordinates
        if (mesh->HasTextureCoords(0)) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    // --- Load Material Texture (diffuse) ---
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

    // Attributes: pos(3) + color(3) + normal(3) + texcoord(2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    indexCount = static_cast<unsigned int>(indices.size());



}

void StaticMeshComp::Draw(const Shader& shader) {
    if (!visible) return;

    shader.use(); // Activate shader

    // --- Pass model matrix ---
    shader.setMat4("model", model);

    // --- Pass material ---
    // Texture
    if (textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("material.diffuse", 0);
    }

    // Specular color (from material or default to white)
    glm::vec3 specColor = glm::vec3(specularStrength);
    shader.setVec3("material.specular", specColor);

    // Shininess
    shader.setFloat("material.shininess", shininess > 0.0f ? shininess : 32.0f);

    // Ambient
    shader.setVec3("material.ambient", ambient);

    // --- Draw ---
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Unbind texture for safety
    glBindTexture(GL_TEXTURE_2D, 0);
}

void StaticMeshComp::loadMaterial(aiMaterial* material) {
    // --- Diffuse Texture ---
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

        std::string filename = str.C_Str();
        std::string texturePath;

        if (filename.find('/') != std::string::npos || filename.find('\\') != std::string::npos)
            texturePath = filename;
        else
            texturePath = "../content/Textures/" + filename;

        textureID = loadTexture(texturePath.c_str());
    } else {
        std::cout << "No diffuse texture found in material\n";
        textureID = 0;
    }

    // --- Ambient Color ---
    aiColor3D ambientColor;
    if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor))
        ambientColor = aiColor3D(0.1f, 0.1f, 0.1f);
    ambient = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);

    // --- Specular Color ---
    aiColor3D specColor;
    if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_SPECULAR, specColor)) {
        specColor = aiColor3D(1.0f, 1.0f, 1.0f); // default
        std::cout << "Failed to get Specular color, using default\n";
    }
    specularStrength = glm::max(glm::max(specColor.r, specColor.g), specColor.b); // use max channel as scalar

    // --- Shininess ---
    float shininessVal;
    if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininessVal)) {
        shininessVal = 32.0f;
        std::cout << "Failed to get shininess, using default 32\n";
    }
    shininess = shininessVal;
}

