// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Entity/Entity.h"
#include "Entity/LightEntity.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderManager.h"
#include "Developer/DevGui.h"
#include "Developer/globals.h"
#include "Components/RadiusLightComp.h"

// Globals
float GlobalLightIntensity = 1.0f;
glm::vec3 GlobalLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 GlobalLightPos = glm::vec3(0.0f, 1.5f, 1.0f);
float GlobalLightRadius = 10.0f;
float GlobalLightGradient = 1.0f;
DevGui Globaldevgui;

// Window settings
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    RenderManager* renderer = reinterpret_cast<RenderManager*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        renderer->OnResize(width, height);
    }
}

int main() {
#pragma region GLFW Setup
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GeoDash3D", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }
#pragma endregion

#pragma region Entity Setup
    Entity test;
    Entity test2;
    LightEntity light;

    light.Scale = glm::vec3(0.1f);
    test2.Position = glm::vec3(0.0f, -2.0f, 1.0f);
#pragma endregion

#pragma region Shader Setup
    Shader basicShader;
    basicShader.recompile();
#pragma endregion

#pragma region Renderer Setup
    RenderManager Renderer;
    float aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
    Camera Cam(90.0f, aspect, 0.1f, 100.0f);

    glfwSetWindowUserPointer(window, &Renderer);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Renderer.SetActiveCamera(&Cam);
    Renderer.AddMesh(&test.StaticMesh);
    Renderer.AddMesh(&light.StaticMesh);
    Renderer.AddMesh(&test2.StaticMesh);
    Renderer.Light = &light;
#pragma endregion

#pragma region DeltaTime Setup
    float currentTime = glfwGetTime();
    float lastFrameTime = currentTime;
    float deltaTime = 0.0f;
#pragma endregion

    Globaldevgui.Init(window);

#pragma region Main Loop
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tick logic
        Renderer.RenderAll(basicShader);
        test.tick(deltaTime);
        test2.tick(deltaTime);
        Globaldevgui.DrawGui(deltaTime);

        light.tick(deltaTime);
        light.Position = GlobalLightPos;
        light.setIntensity(GlobalLightIntensity);
        light.setColor(GlobalLightColor);
        light.setRadius(GlobalLightRadius);
        light.setGradient(GlobalLightGradient);

#pragma region Camera Controls
        float moveSpeed = 5.0f;
        float rotateSpeed = 90.0f;

        glm::vec3 forward = glm::normalize(Cam.Front);
        glm::vec3 right   = glm::normalize(glm::cross(forward, Cam.Up));
        glm::vec3 up      = Cam.Up;

        if (glfwGetKey(window, GLFW_KEY_W)) Cam.Position += forward * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S)) Cam.Position -= forward * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D)) Cam.Position += right * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A)) Cam.Position -= right * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_SPACE)) Cam.Position += up * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) Cam.Position -= up * moveSpeed * deltaTime;

        float yaw = 0.0f, pitch = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT))  yaw   = -rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT))   yaw   =  rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP))     pitch =  rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN))   pitch = -rotateSpeed * deltaTime;

        if (yaw != 0.0f || pitch != 0.0f) {
            glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), Cam.Up);
            glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right);

            glm::vec4 newFront = pitchRot * yawRot * glm::vec4(forward, 0.0f);
            Cam.Front = glm::normalize(glm::vec3(newFront));
        }

        if (auto Controller = test.GetComponent<ControllerComponent>()) {
            Controller->tick(deltaTime);
        }
#pragma endregion

        glfwSwapBuffers(window);
        glfwPollEvents();

#pragma region Dev GUI
        if (Globaldevgui.toggleCulling) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        } else {
            glDisable(GL_CULL_FACE);
        }

        if (Globaldevgui.recompileShaders) {
            basicShader.recompile();
            Globaldevgui.recompileShaders = false;
            Globaldevgui.LogMessage = "Shaders recompiled";
        }
#pragma endregion
    }
#pragma endregion

    Globaldevgui.Cleanup();
    glfwTerminate();
    return 0;
}
