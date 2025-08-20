//
// Created by Ben on 15/08/2025.
//

#include "ControllerComponent.h"

#include "Entity.h"

void ControllerComponent::tick(float deltaTime) {


    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && Owner->Position.y <= 0) {
        Owner->Velocity += glm::vec3(0, 0.2, 0);
    }
}

void ControllerComponent::init() {
    window = glfwGetCurrentContext();
}
