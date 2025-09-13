#include "RenderManager.h"
#include <glad/glad.h>
#include "../Developer/globals.h"
#include <glm/gtc/matrix_transform.hpp>

void RenderManager::RenderAll(Shader& shader) {
    if (!ActiveCamera) return;

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
}
