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

// Window settings
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

//TEMP LOADING SHADERS
std::string LoadShaderSource(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

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
//Testing Entities
    Entity test; // Test entity
    Entity test2; // Test 2 entity
    test2.Position = glm::vec3(10, 0, 0);

#pragma region Shader Tests

    Shader basicShader(
    LoadShaderSource("../shaders/basic.vert").c_str(),
    LoadShaderSource("../shaders/basic.frag").c_str()
);

#pragma endregion

    RenderManager Renderer;
    float aspect = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
    Camera Cam(90.0f, aspect, 0.1f, 100.0f);

    Renderer.SetActiveCamera(&Cam); // sets active camera
    Renderer.AddMesh(&test.StaticMesh); // adds test mesh to renderer
    Renderer.AddMesh(&test2.StaticMesh); // adds test mesh to renderer
    float currentTime = glfwGetTime();
    float lastFrameTime = currentTime;
    float deltaTime = currentTime - lastFrameTime;



    //IMGUI object
    DevGui GUI(window);



    //MAIN LOOP
    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        glEnable(GL_DEPTH_TEST); //depth testing

        //Background
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Tick Logic

        Renderer.RenderAll(basicShader);
        test.tick(deltaTime);
        test2.tick(deltaTime);
        GUI.DrawGui(deltaTime);


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

        if (GUI.toggleCulling) { //toggle in ImGUI
            glEnable(GL_CULL_FACE); // enable culling
            glCullFace(GL_BACK); // cull back faces
            glFrontFace(GL_CCW); // counter-clockwise winding = front face
        } else {
            glDisable(GL_CULL_FACE); //disable culling
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear depth

        if (GUI.recompileShaders) {
            basicShader.recompile(
        LoadShaderSource("../shaders/basic.vert").c_str(),
        LoadShaderSource("../shaders/basic.frag").c_str()
            );
            GUI.recompileShaders = false;
        }

    }


    //GUI cleanup
    GUI.Cleanup();

    glfwTerminate();
    return 0;
}
