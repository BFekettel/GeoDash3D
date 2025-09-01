#include "RenderManager.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void RenderManager::RenderAll(Shader& shader) {
    if (!ActiveCamera) return;

    glm::mat4 view = ActiveCamera->GetView();
    glm::mat4 projection = ActiveCamera->GetProjection();

    for (auto* mesh : Meshes) {
        mesh->Draw(shader, view, projection);
    }
}
