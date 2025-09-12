//
// Created by Ben on 1/09/2025.
//

#include "DevGui.h"
#include "globals.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>

DevGui::DevGui(GLFWwindow* window) {
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void DevGui::Cleanup() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DevGui::DrawGui(float &deltaTime) {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw frame stats
    ImGui::Begin("Stats");
    ImGui::Text("Frame time: %.2f ms", deltaTime * 1000.0f);
    ImGui::Text("FPS: %.1f", 1.0 / deltaTime);
    ImGui::Checkbox("Toggle Culling", &toggleCulling);

    if (ImGui::Button("Recompile Shaders", ImVec2(-1, 0))) {
        recompileShaders = true;
    }

    ImGui::Separator();
    ImGui::SliderFloat("Ambient Strength", &GlobalAmbientStrength, 0.0f, 1.0f);
    ImGui::Separator();
    ImGui::SliderFloat("Global Light Color R", &GlobalLightColor.r, 0.0f, 1.0f);
    ImGui::SliderFloat("Global Light Color G", &GlobalLightColor.g, 0.0f, 1.0f);
    ImGui::SliderFloat("Global Light Color B", &GlobalLightColor.b, 0.0f, 1.0f);
    ImGui::Separator();
    ImGui::DragFloat("Global Light Location X", &GlobalLightPos.x, 0.1f);
    ImGui::DragFloat("Global Light Location Y", &GlobalLightPos.y, 0.1f);
    ImGui::DragFloat("Global Light Location Z", &GlobalLightPos.z, 0.1f);
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
