// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for translate, rotate, perspective
#include <glm/gtc/type_ptr.hpp>         // for value_ptr

#include "Entity.h"

#include <cmath>

// Window settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main() {
    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLFW Cube", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }

    Entity test; // Test entity



    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);



    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { //generic W Pressed
            test.Position += glm::vec3(0.0f, 0.01f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { //generic S Pressed
            test.Position += glm::vec3(0.0f, -0.01f, 0.0f);
        }
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(test.StaticMesh.shaderProgram);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::translate(glm::mat4(1.0f), test.Position);

        int modelLoc = glGetUniformLocation(test.StaticMesh.shaderProgram, "model");
        int viewLoc = glGetUniformLocation(test.StaticMesh.shaderProgram, "view");
        int projLoc = glGetUniformLocation(test.StaticMesh.shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(test.StaticMesh.VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &test.StaticMesh.VAO);
    glDeleteBuffers(1, &test.StaticMesh.VBO);
    glDeleteBuffers(1, &test.StaticMesh.EBO);

    glfwTerminate();
    return 0;
}
