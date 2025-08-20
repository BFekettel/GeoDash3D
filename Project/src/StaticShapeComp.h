//
// Created by Ben on 14/08/2025.
//

#ifndef PROJECT_STATICMESHCOMP_H
#define PROJECT_STATICMESHCOMP_H


#include "Component.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

class StaticShapeComp: public Component {
public:
    unsigned int VAO{}, VBO{}, EBO{};
    StaticShapeComp();
    // Cube vertices (position + color)

#pragma region Model Information
    float Vertices[48] = {
        // positions         // colors
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.3f, 0.5f, 0.7f
    };

    unsigned int Indices[36] = {
        0, 1, 2, 2, 3, 0, // back
        4, 5, 6, 6, 7, 4, // front
        0, 4, 7, 7, 3, 0, // left
        1, 5, 6, 6, 2, 1, // right
        3, 2, 6, 6, 7, 3, // top
        0, 1, 5, 5, 4, 0  // bottom
    };
#pragma endregion

    //Tick events
    void tick(float deltaTime) override;
    void Draw(const glm::mat4& view, const glm::mat4& projection);


#pragma region Shader Logic
    // Simple vertex shader
    const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
)";

    // Simple fragment shader
    const char* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(ourColor, 1.0);
}
)";

    // Error checking for shaders
    static unsigned int compileShader(unsigned int type, const char* source) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compile error: " << infoLog << std::endl;
        }
        return shader;
    }

    [[nodiscard]] unsigned int createShaderProgram() const;
#pragma endregion

    //Shader Program
    unsigned int shaderProgram;

private:
    int modelLoc;
    int viewLoc;
    int projLoc;
    int indexCount;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1200 / 800, 0.1f, 100.0f); //TODO: change aspect to something not using magic numbers

};


#endif //PROJECT_STATICMESHCOMP_H