//
// Created by Ben on 14/08/2025.
//

#ifndef PROJECT_ENTITY_H
#define PROJECT_ENTITY_H

#include <iostream>
#include <typeindex>
#include <unordered_map>

#include "StaticShapeComp.h"
#include "Component.h"
#include "ControllerComponent.h"

#include <glm/glm.hpp>

class Entity {
public:
    // Translation
    glm::vec3 Position = {0.0f, 0.0f, 1.0f};
    glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    glm::vec3 Velocity = {0.0f, 0.0f, 0.0f}; // TEMP CURRENT VELOCITY

    //Temp Static Component
    StaticShapeComp StaticMesh;

    //Constructor & tick
    Entity();
    void tick(float deltaTime);

    //Component templates
    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->SetOwner(this);
        comp->init();

        T& ref = *comp;
        components[typeid(T)] = std::move(comp);
        return ref;
    }

    template <typename T>
    T* GetComponent()
    {
        auto it = components.find(typeid(T));
        if (it != components.end())
        {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }


private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

};


#endif //PROJECT_ENTITY_H