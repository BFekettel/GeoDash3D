// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Entity/Entity.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderManager.h"
#include "Developer/DevGui.h"
#include "Developer/globals.h"

// Globals
float GlobalLightIntensity = 1.0f;
float GlobalLightRadius = 10.0f;
glm::vec3 GlobalLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 GlobalLightPos = glm::vec3(0.0f, -45.0f, 1.0f);
float GlobalLightGradient = 17.5f;
DevGui Globaldevgui;

// Window settings
const unsigned int WIDTH  = 1200;
const unsigned int HEIGHT = 800;

// Callback for window resize
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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLFW Cube", nullptr, nullptr);
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

#pragma region Test Entities + Lights
    Entity test;
    Entity test2;
    Entity test3;
    Entity test4;

    test2.Position = glm::vec3(0.0f, -2.0f, 1.0f);
    test3.Position = glm::vec3(2.0f, -2.0f, 1.0f);
    test4.Position = glm::vec3(4.0f, -2.0f, 1.0f);

    LightEntity globalLight;
    globalLight.Type = LightType::Global;   // only global/spot cast shadows
    globalLight.Position = GlobalLightPos;
    globalLight.setColor(GlobalLightColor);
    globalLight.setIntensity(GlobalLightIntensity);
#pragma endregion

#pragma region Shaders
    Shader sceneShader;
    sceneShader.compile("../shaders/default.vert", "../shaders/default.frag");

    Shader shadowShader;
    shadowShader.compile("../shaders/shadow_depth.vs", "../shaders/shadow_depth.fs");
#pragma endregion

#pragma region Renderer Setup
    RenderManager Renderer;
    float aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
    Camera Cam(90.0f, aspect, 0.1f, 100.0f);
    Renderer.SetActiveCamera(&Cam);
    Renderer.AddMesh(&test.StaticMesh);
    Renderer.AddMesh(&test2.StaticMesh);
    Renderer.AddMesh(&test3.StaticMesh);
    Renderer.AddMesh(&test4.StaticMesh);
    Renderer.AddLight(&globalLight);

    glfwSetWindowUserPointer(window, &Renderer);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


#pragma endregion

#pragma region Time Setup
    float currentTime   = glfwGetTime();
    float lastFrameTime = currentTime;
    float deltaTime     = 0.0f;
#pragma endregion


    Globaldevgui.Init(window);

    // =============================
    // Main Loop
    // =============================
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime   = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        glEnable(GL_DEPTH_TEST);

        // Background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Scene + Shadows ---
        Renderer.RenderAll(sceneShader, shadowShader);

        // --- Entity Logic ---
        test.tick(deltaTime);
        test2.tick(deltaTime);
        test3.tick(deltaTime);
        test4.tick(deltaTime);
        if (auto Controller = test.GetComponent<ControllerComponent>()) {
            Controller->tick(deltaTime);
        }
        globalLight.tick(deltaTime);
        globalLight.setDirection(GlobalLightPos.x, GlobalLightPos.y);
        globalLight.setColor(GlobalLightColor);
        globalLight.setIntensity(GlobalLightIntensity);

        // --- Dev GUI ---
        Globaldevgui.DrawGui(deltaTime);

#pragma region Camera Controls
        float moveSpeed   = 5.0f;
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
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) yaw   = -rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT))  yaw   =  rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP))    pitch =  rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN))  pitch = -rotateSpeed * deltaTime;

        if (yaw != 0.0f || pitch != 0.0f) {
            glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), Cam.Up);
            glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right);
            glm::vec4 newFront = pitchRot * yawRot * glm::vec4(forward, 0.0f);
            Cam.Front = glm::normalize(glm::vec3(newFront));
        }
#pragma endregion

        // --- Shader hot-reload ---
        if (Globaldevgui.recompileShaders) {
            sceneShader.compile("../shaders/default.vert", "../shaders/default.frag");
            shadowShader.compile("../shaders/shadow_depth.vs", "../shaders/shadow_depth.fs");
            Globaldevgui.recompileShaders = false;
            Globaldevgui.LogMessage = "Shaders Compiled";
        }

        // --- Face culling toggle ---
        if (Globaldevgui.toggleCulling) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        } else {
            glDisable(GL_CULL_FACE);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Globaldevgui.Cleanup();
    glfwTerminate();
    return 0;
}
