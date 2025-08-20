//
// Created by Ben on 19/08/2025.
//



#ifndef PROJECT_RENDERMANAGER_H
#define PROJECT_RENDERMANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <iostream>


class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    float fov, aspect, nearClip, farClip;

    Camera(float fov, float aspect, float nearClip, float farClip)
        : Position(0.0f, 0.0f, 3.0f),
          Front(0.0f, 0.0f, -1.0f),
          Up(0.0f, 1.0f, 0.0f),
          fov(fov), aspect(aspect), nearClip(nearClip), farClip(farClip) {}

    glm::mat4 GetProjection() const {
        return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
    }

    glm::mat4 GetView() const {
        return glm::lookAt(Position, Position + Front, Up);
    }
};


#endif //PROJECT_RENDERMANAGER_H