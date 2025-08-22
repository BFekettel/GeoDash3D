// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for translate, rotate, perspective
#include <glm/gtc/type_ptr.hpp>         // for value_ptr

#include "Entity/Entity.h"

#include <cmath>

#include "Rendering/Camera.h"
#include "Rendering/RenderManager.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");



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

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Draw frame stats
        ImGui::Begin("Stats");
        ImGui::Text("Frame time: %.2f ms", deltaTime * 1000.0f);
        ImGui::Text("FPS: %.1f", 1.0 / deltaTime);
        ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    // Cleanup Window
    glDeleteVertexArrays(1, &test.StaticMesh.VAO);
    glDeleteBuffers(1, &test.StaticMesh.VBO);
    glDeleteBuffers(1, &test.StaticMesh.EBO);

    glfwTerminate();
    return 0;
}
