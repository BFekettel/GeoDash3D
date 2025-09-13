//
// Created by Ben on 14/08/2025.
//

#ifndef PROJECT_STATICMESHCOMP_H
#define PROJECT_STATICMESHCOMP_H
#include "../Rendering/Shader.h"

#include "Component.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

class StaticMeshComp: public Component {
public:
    unsigned int VAO{}, VBO{}, EBO{};
    glm::mat4 model{1.0f}; //Uniform for model

    StaticMeshComp();
    ~StaticMeshComp();

    unsigned int loadTexture(const char *filename);

    void loadModel(const char* path);

    void tick(float deltaTime) override {}
    void Draw(const Shader& shader);

    bool visible = true;

private:
    int indexCount;
    unsigned int textureID;
    float shininess;
    float specularStrength;

};



#endif //PROJECT_STATICMESHCOMP_H