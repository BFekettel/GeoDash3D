//
// Created by Ben on 15/08/2025.
//

#ifndef PROJECT_CONTROLLERCOMPONENET_H
#define PROJECT_CONTROLLERCOMPONENET_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Component.h"

class ControllerComponent: public Component {
public:
    void tick(float deltaTime) override;
    void init() override;

    ControllerComponent() {};

private:
    GLFWwindow* window;
};


#endif //PROJECT_CONTROLLERCOMPONENET_H