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
#include "Components/RadiusLightComp.h"

//Globals
float GlobalLightIntensity = 1.0f;
glm::vec3 GlobalLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 GlobalLightPos = glm::vec3(0.0f, 1.5f, 1.0f);
float GlobalLightRadius = 10.0f;
float GlobalLightGradient = 1.0f;
DevGui Globaldevgui;


// Window settings
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

//TEMP LOADING SHADERS TODO: add to shader object



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    RenderManager* renderer = reinterpret_cast<RenderManager*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        renderer->OnResize(width, height);
    }
}


int main() {
#pragma region GLFW Setup
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
    glfwSwapInterval(1); // Frame limter
    /*
     * 1 = monitor speed
     * 0 = uncapped
     * 2 = 50% cap
     */

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }

#pragma endregion

//Testing Entities
    Entity test; // Test entity
    LightEntity light; // Test Light

    light.Scale = glm::vec3(0.1f, 0.1f, 0.1f);
    light.AddComponent<RadiusLightComp>(); //Adds light component


#pragma region Shader Tests

    Shader basicShader;
    basicShader.recompile();

#pragma endregion

#pragma region Renderer Setup
    RenderManager Renderer;
    float aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
    Camera Cam(90.0f, aspect, 0.1f, 100.0f);

    glfwSetWindowUserPointer(window, &Renderer);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Renderer.SetActiveCamera(&Cam); // sets active camera
    Renderer.AddMesh(&test.StaticMesh); // adds test mesh to renderer
    Renderer.AddMesh(&light.StaticMesh);
    Renderer.Light = &light;

#pragma endregion

#pragma region Delta Setup
    float currentTime = glfwGetTime();
    float lastFrameTime = currentTime;
    float deltaTime = currentTime - lastFrameTime;



#pragma endregion
    //IMGUI object

    Globaldevgui.Init(window);

    //MAIN LOOP
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        glEnable(GL_DEPTH_TEST); //depth testing

        //Background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Tick Logic

        Renderer.RenderAll(basicShader);
        test.tick(deltaTime);
        Globaldevgui.DrawGui(deltaTime);

        light.tick(deltaTime);
        light.Position = GlobalLightPos;
        light.setIntensity(GlobalLightIntensity);
        light.setColor(GlobalLightColor);
        light.setRadius(GlobalLightRadius);
        light.setGradient(GlobalLightGradient);


#pragma region Camera Movement
        // Camera speed
        float moveSpeed = 5.0f;       // units per second
        float rotateSpeed = 90.0f;    // degrees per second

        // Movement input (frame-rate independent)
        glm::vec3 forward = glm::normalize(Cam.Front);
        glm::vec3 right   = glm::normalize(glm::cross(forward, Cam.Up));
        glm::vec3 up      = Cam.Up;

        if (glfwGetKey(window, GLFW_KEY_W)) Cam.Position += forward * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S)) Cam.Position -= forward * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D)) Cam.Position += right * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A)) Cam.Position -= right * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_SPACE)) Cam.Position += up * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) Cam.Position -= up * moveSpeed * deltaTime;

        // Rotation input (using arrow keys)
        float yaw   = 0.0f;
        float pitch = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT))  yaw   = -rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT)) yaw   =  rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP))    pitch =  rotateSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN))  pitch = -rotateSpeed * deltaTime;

        // Apply rotation
        if (yaw != 0.0f || pitch != 0.0f) {
            // Yaw: rotate around Up vector
            glm::mat4 yawRot   = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), Cam.Up);
            // Pitch: rotate around Right vector
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


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear depth

#pragma region Dev GUI
        if (Globaldevgui.toggleCulling) { //toggle in ImGUI
            glEnable(GL_CULL_FACE); // enable culling
            glCullFace(GL_BACK); // cull back faces
            glFrontFace(GL_CCW); // counter-clockwise winding = front face
        } else {
            glDisable(GL_CULL_FACE); //disable culling
        }



        if (Globaldevgui.recompileShaders) {
            basicShader.recompile();
            Globaldevgui.recompileShaders = false;
            Globaldevgui.LogMessage = "Shaders Compiled";
        }
#pragma endregion

    }


    //GUI cleanup
    Globaldevgui.Cleanup();

    glfwTerminate();
    return 0;
}
