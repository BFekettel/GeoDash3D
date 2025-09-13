#include "RenderManager.h"
#include <GLFW/glfw3.h>
#include "../Developer/globals.h"
#include <glm/gtc/matrix_transform.hpp>

void RenderManager::RenderAll(Shader& shader) {
    if (!ActiveCamera) return;
    //delta time
    currentTime = glfwGetTime();
    glm::mat4 view = ActiveCamera->GetView();
    glm::mat4 projection = ActiveCamera->GetProjection();
    /*
     * Uniforms for shaders
     */
    // Scene data
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setFloat("ambientStrength", GlobalAmbientStrength);
    shader.setVec3("lightColor", GlobalLightColor);
    shader.setVec3("lightPos", GlobalLightPos);
    shader.setVec3("viewPos", ActiveCamera->Position);

    for (auto* mesh : Meshes) {
        mesh->Draw(shader);
    }

    //this is to test the tick time for rendering
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    Globaldevgui.drawTime = deltaTime;
}
