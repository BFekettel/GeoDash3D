// src/Components/Component.h
// Created by Ben on 14/08/2025.
// Header contains declarations only — definitions are in Component.cpp

#ifndef PROJECT_COMPONENT_H
#define PROJECT_COMPONENT_H

#pragma once

class Entity;

class Component {
public:
    // default ctor for some components that might be value members
    Component();
    explicit Component(Entity* owner);
    virtual ~Component();

    // Project uses lowercase lifecycle names in many files
    virtual void init();
    virtual void destroy();
    virtual void tick(float dt);

    // Owner helpers (project code expects SetOwner/GetOwner)
    void SetOwner(Entity* owner);
    Entity* GetOwner() const;

protected:
    Entity* Owner = nullptr;
};

#endif // PROJECT_COMPONENT_H
