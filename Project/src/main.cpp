// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for translate, rotate, perspective
#include <glm/gtc/type_ptr.hpp>         // for value_ptr

#include "Entity.h"

#include <cmath>

#include "Camera.h"
#include "RenderManager.h"

// Window settings
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

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
    Entity test2; // Test 2 entity
    test2.Position = glm::vec3(10, 0, 0);


    RenderManager Renderer;
    float aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
    Camera Cam(90.0f, aspect, 0.1f, 100.0f);

    Renderer.SetActiveCamera(&Cam); // sets active camera
    Renderer.AddMesh(&test.StaticMesh); // adds test mesh to renderer
    Renderer.AddMesh(&test2.StaticMesh); // adds test mesh to renderer
    float currentTime = glfwGetTime();
    float lastFrameTime = currentTime;
    float deltaTime = currentTime - lastFrameTime;



    //MAIN LOOP
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        //Background
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Tick Logic
        Renderer.RenderAll();
        test.tick(deltaTime);

        //TEST: camera movement
        if (glfwGetKey(window, GLFW_KEY_W)) {
            Cam.Position += glm::vec3(0.0f, 0.01f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            Cam.Position -= glm::vec3(0.0f, 0.01f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            Cam.Position += glm::vec3(0.01f, 0.0f, 0.00f);
        }
        if (glfwGetKey(window, GLFW_KEY_A)) {
            Cam.Position -= glm::vec3(0.01f, 0.0f, 0.00f);
        }

        if (glfwGetKey(window, GLFW_KEY_UP)) {
            Cam.Front += glm::vec3(0.0f, 0.05f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN)) {
            Cam.Front -= glm::vec3(0.0f, 0.05f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
            Cam.Front += glm::vec3(0.05f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT)) {
            Cam.Front -= glm::vec3(0.05f, 0.0f, 0.0f);
        }

        if (auto Controller = test.GetComponent<ControllerComponent>()) {
            Controller->tick(deltaTime);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &test.StaticMesh.VAO);
    glDeleteBuffers(1, &test.StaticMesh.VBO);
    glDeleteBuffers(1, &test.StaticMesh.EBO);

    glfwTerminate();
    return 0;
}
