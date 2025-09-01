//
// Created by Ben on 1/09/2025.
//

#ifndef PROJECT_DEVGUI_H
#define PROJECT_DEVGUI_H

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

class DevGui {

    public:
    explicit DevGui(GLFWwindow* window);
    ~DevGui() = default;

    bool toggleCulling = true;
    bool recompileShaders = false;

    void Cleanup();
    void DrawGui(float &deltaTime);
};


#endif //PROJECT_DEVGUI_H