#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <vector>
#include "Camera.h"
#include "StaticShapeComp.h"

class RenderManager {
public:
    // static RenderManager& GetInstance() {
    //     static RenderManager instance;
    //     return instance;
    // }

    RenderManager() = default;

    // Camera
    Camera* ActiveCamera = nullptr;

    // Rendering components
    std::vector<StaticShapeComp*> Meshes;

    void SetActiveCamera(Camera* cam) { ActiveCamera = cam; }
    void AddMesh(StaticShapeComp* mesh) { Meshes.push_back(mesh); }
    void RenderAll();

private:

};

#endif