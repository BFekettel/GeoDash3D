//
// Created by Ben on 14/08/2025.
//

#include "Entity.h"

#include "Components/PhysicsComp.h"


Entity::Entity() {
    StaticMesh.SetOwner(this);

    auto& Controller = AddComponent<ControllerComponent>(); // Adds controller to entity

}

void Entity::tick(float deltaTime) { //TODO: create physics manager


    if (!GetComponent<PhysicsComp>()) {
        Position += Velocity * deltaTime;   //keep legacy behaviour for non-physics entities
    }

    // if (Position.y > 0) {
    //     Velocity.y -= 0.01;
    // } else if (Position.y <= 0) {
    //     Velocity.y = 0;
    // }
    //Updates Position of mesh
    glm::mat4 model(1.0f);
    model = glm::translate(model, Position);
    model = glm::scale(model, Scale);
    //this needs fixing eventually
    //model = glm::Mat4(Rotation);
    StaticMesh.model = model;

}
