﻿#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <vector>
#include "Camera.h"
#include "../Components/StaticShapeComp.h"
#include "Shader.h"

class RenderManager {
public:
    RenderManager() = default;

    // Camera
    Camera* ActiveCamera = nullptr;

    // Rendering components
    std::vector<StaticShapeComp*> Meshes;

    void SetActiveCamera(Camera* cam) { ActiveCamera = cam; }
    void AddMesh(StaticShapeComp* mesh) { Meshes.push_back(mesh); }
    void RenderAll(Shader& shader);

private:

};

#endif