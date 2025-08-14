//
// Created by Ben on 14/08/2025.
//

#ifndef PROJECT_ENTITY_H
#define PROJECT_ENTITY_H

#include <iostream>
#include "StaticShapeComp.h"
#include <glm/glm.hpp>

class Entity {
public:

    glm::vec3 Position = {0.0f, 0.0f, -3.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    StaticShapeComp StaticMesh;

    Entity() {StaticMesh.SetOwner(this);}

};


#endif //PROJECT_ENTITY_H