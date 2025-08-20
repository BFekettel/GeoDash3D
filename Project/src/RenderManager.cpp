#include "RenderManager.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void RenderManager::RenderAll() {
    if (!ActiveCamera) return;

    glm::mat4 projection = ActiveCamera->GetProjection();
    glm::mat4 view = ActiveCamera->GetView();

    for (auto* mesh : Meshes) {
        if (mesh)
            mesh->Draw(view, projection);
    }
}
