//
// Created by Ben on 14/08/2025.
//

#ifndef PROJECT_COMPONENT_H
#define PROJECT_COMPONENT_H


class Entity;

class Component {

public:
    Component();
    virtual ~Component();
    Entity* Owner = nullptr;
    virtual void init(); // runs on added to owner
    virtual void tick(float deltaTime); // runs each frame

    void SetOwner(Entity* owner);
};




#endif //PROJECT_COMPONENT_H
