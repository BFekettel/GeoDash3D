// src/Components/Component.cpp
#include "Component.h"

Component::Component() = default;
Component::Component(Entity* owner) : Owner(owner) {}
Component::~Component() = default;

void Component::init() {}
void Component::destroy() {}
void Component::tick(float) {}
void Component::SetOwner(Entity* owner) { Owner = owner; }
Entity* Component::GetOwner() const { return Owner; }
