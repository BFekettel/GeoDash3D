//
// Created by Ben on 14/08/2025.
//

#include "Component.h"


Component::Component() = default;

Component::~Component() = default;

void Component::init() {
}

void Component::tick(float deltaTime) {
}

void Component::SetOwner(Entity *owner) { this->Owner = owner; }
