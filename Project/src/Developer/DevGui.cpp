//
// Created by Ben on 1/09/2025.
//

#include "DevGui.h"
#include "globals.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>

// DevGui::DevGui(GLFWwindow* window) {
//     // Setup ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     ImGui::StyleColorsDark();
//
//     // Setup Platform/Renderer bindings
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init("#version 330");
// }

DevGui::DevGui() : window(nullptr), initialized(false) {}



void DevGui::Init(GLFWwindow* win) {
    if (initialized) return; // prevent double init
    window = win;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    initialized = true;
}

void DevGui::Cleanup() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DevGui::DrawGui(float &deltaTime) {
    if (!initialized) return;
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw frame stats
    ImGui::Begin("Stats");

    // Stats
    if (ImGui::CollapsingHeader("Stats")) {
        ImGui::Text("Frame time: %.2f ms", deltaTime * 1000.0f);
        ImGui::Text("FPS: %.1f", 1.0 / deltaTime);
        ImGui::Text("Draw time: %.2f ms", drawTime * 1000.0f);
    }
    // Options
    if (ImGui::CollapsingHeader("Options")) {
        if (ImGui::Button ("Toggle Culling", ImVec2(-1,0))) {
            toggleCulling = !toggleCulling;
            if (toggleCulling) {
                const char *message = "Toggle Culling: On";
                LogMessage = message;
            } else {
                const char *message = "Toggle Culling: Off";
                LogMessage = message;
            }
        }


        if (ImGui::Button("Recompile Shaders", ImVec2(-1, 0))) {
            const char *message = "Recompiling Shaders...";
            LogMessage = message;
            recompileShaders = true;
        }
    }

    // Light testing
    if (ImGui::CollapsingHeader("Light testing")) {
        ImGui::DragFloat("Light Intensity", &GlobalLightIntensity, 0.1f);
        ImGui::Separator();
        ImGui::SliderFloat("Global Light Color R", &GlobalLightColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat("Global Light Color G", &GlobalLightColor.g, 0.0f, 1.0f);
        ImGui::SliderFloat("Global Light Color B", &GlobalLightColor.b, 0.0f, 1.0f);
        ImGui::Separator();
        ImGui::DragFloat("Global Light Location X", &GlobalLightPos.x, 0.1f);
        ImGui::DragFloat("Global Light Location Y", &GlobalLightPos.y, 0.1f);
        ImGui::DragFloat("Global Light Location Z", &GlobalLightPos.z, 0.1f);
        ImGui::Separator();
        ImGui::DragFloat("Global Light Radius", &GlobalLightRadius, 0.1f);
        ImGui::SliderFloat("Global Light Gradient", &GlobalLightGradient, 0.0f, 1.0f);
    }
    ImGui::Text("Log: ");
    ImGui::Text(LogMessage);


    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
