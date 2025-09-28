#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Shader.h"
#include "../Entity/LightEntity.h"
#include "../Components/StaticMeshComp.h"

struct ShadowData {
    unsigned int FBO = 0;
    unsigned int depthMap = 0;
};

class RenderManager {
public:
    RenderManager();

    void AddMesh(StaticMeshComp* mesh) { Meshes.push_back(mesh); }
    void AddLight(LightEntity* light) { Lights.push_back(light); }
    void SetActiveCamera(Camera* cam) { ActiveCamera = cam; }

    void RenderAll(Shader& sceneShader, Shader& shadowShader);
    void OnResize(int w, int h);

    Camera* ActiveCamera = nullptr;

private:
    void EnsureShadowResources();
    void RenderShadowPass(size_t i, Shader& shadowShader);


    std::vector<StaticMeshComp*> Meshes;
    std::vector<LightEntity*> Lights;
    std::vector<ShadowData> shadowMaps;
    std::vector<glm::mat4> lightSpaceMats;

    int windowWidth = 1200, windowHeight = 800;
    float currentTime = 0, lastFrameTime = 0, deltaTime = 0;

    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;
};
