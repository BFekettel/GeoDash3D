//
// Created by Ben on 14/08/2025.
//

#ifndef PROJECT_STATICMESHCOMP_H
#define PROJECT_STATICMESHCOMP_H
#include "../Rendering/Shader.h"

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../external/stb/stb_image.h"

class StaticShapeComp: public Component {
public:
    unsigned int VAO{}, VBO{}, EBO{};
    unsigned int texture;
    glm::mat4 model{1.0f}; //Uniform for model
    /*
     * Uniforms allow cpu members be passed into gpu shader programs
     */

    StaticShapeComp();
    ~StaticShapeComp();

    void tick(float deltaTime) override {}
    void Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection);

private:
    int indexCount;


#pragma region Model Information
    float vertices[6*8] = {
        // positions          // colors
        -0.5f,-0.5f,-0.5f,   1.0f,1.0f,1.0f,
         0.5f,-0.5f,-0.5f,   1.0f,1.0f,1.0f,
         0.5f, 0.5f,-0.5f,   1.0f,1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,   1.0f,1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,   1.0f,1.0f,1.0f,
         0.5f,-0.5f, 0.5f,   1.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f,   1.0f,1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,   1.0f,1.0f,1.0f
    };

    unsigned int Indices[36] = { //triangles
        0, 2, 1, 0, 3, 2, // back
        4, 5, 6, 4, 6, 7, // front
        0, 1, 5, 0, 5, 4, // left
        2, 3, 7, 2, 7, 6, // right
        3, 0, 4, 3, 4, 7, // top
        1, 2, 6, 1, 6, 5  // bottom
    };

    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("resources/Denzel.jpg", &width, &height, &nrChannels, 0);


#pragma endregion
};



#endif //PROJECT_STATICMESHCOMP_H