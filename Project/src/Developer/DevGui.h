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
    DevGui();
    ~DevGui() = default;
    void Init(GLFWwindow* window);
    //explicit DevGui(GLFWwindow* window);


    bool toggleCulling = true;
    bool recompileShaders = false;
    const char *LogMessage = "Message";
    float drawTime = 0.0f;

    void Cleanup();
    void DrawGui(float &deltaTime);
    void Tick();

private:
    GLFWwindow* window = nullptr;
    bool initialized = false;
};


#endif //PROJECT_DEVGUI_H