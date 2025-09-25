#include "RenderManager.h"
#include <GLFW/glfw3.h>
#include "../Developer/globals.h"
#include <glm/gtc/matrix_transform.hpp>

void RenderManager::RenderAll(Shader& shader) {
    if (!ActiveCamera) return;

    currentTime = glfwGetTime();
    glm::mat4 view = ActiveCamera->GetView();
    glm::mat4 projection = ActiveCamera->GetProjection();

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", ActiveCamera->Position);

    // Pass number of lights
    shader.setInt("numLights", static_cast<int>(Lights.size()));

    // Loop through lights
    for (size_t i = 0; i < Lights.size(); i++) {
        std::string prefix = "lights[" + std::to_string(i) + "].";

        LightEntity* L = Lights[i];
        shader.setInt(prefix + "type", static_cast<int>(L->Type));
        shader.setVec3(prefix + "position", L->Position);
        shader.setVec3(prefix + "direction", L->getDirection());
        shader.setVec3(prefix + "color", L->getColor());
        shader.setFloat(prefix + "intensity", L->getIntensity());
        shader.setFloat(prefix + "radius", L->getRadius());
        shader.setFloat(prefix + "cutOff", L->getCutOff());
        shader.setFloat(prefix + "outerCutOff", L->getOuterCutOff());
    }

    // Draw all meshes
    for (auto* mesh : Meshes) {
        mesh->Draw(shader);
    }

    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    Globaldevgui.drawTime = deltaTime;
}

