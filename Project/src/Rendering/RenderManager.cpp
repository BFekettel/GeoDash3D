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

    // Pass light data
    shader.setVec3("lightPos", Light->Position);
    shader.setVec3("lightColor", Light->getColor());
    shader.setFloat("lightRadius", Light->getRadius());
    shader.setFloat("lightGradient", Light->getGradient());
    shader.setFloat("lightIntensity", Light->getIntensity());

    for (auto* mesh : Meshes) {
        mesh->Draw(shader);
    }

    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    Globaldevgui.drawTime = deltaTime;
}
