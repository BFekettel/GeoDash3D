#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <vector>
#include "Camera.h"
#include "../Components/StaticMeshComp.h"
#include "Shader.h"
#include "../Entity/LightEntity.h"

class RenderManager {
public:
    RenderManager() = default;

    // Camera
    Camera* ActiveCamera = nullptr;

    // Rendering components
    std::vector<StaticMeshComp*> Meshes;

    void SetActiveCamera(Camera* cam) { ActiveCamera = cam; }
    void AddMesh(StaticMeshComp* mesh) { Meshes.push_back(mesh); }
    void RenderAll(Shader& shader);

    // Handle window resizing
    void OnResize(int width, int height) {
        glViewport(0, 0, width, height);
        if (ActiveCamera && height > 0) {
            float aspect = static_cast<float>(width) / static_cast<float>(height);
            ActiveCamera->SetAspect(aspect);
        }
    }

    LightEntity* Light; //TEMP TODO: make this... better? need to be able to hold multiple lights lol

private:
    float currentTime = glfwGetTime();
    float lastFrameTime = currentTime;
    float deltaTime = currentTime - lastFrameTime;

};

#endif
