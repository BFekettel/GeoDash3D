//
// Created by chee7 on 9/26/25.
//

#ifndef GEODASH3D_PHYSICSMANAGER_H
#define GEODASH3D_PHYSICSMANAGER_H
// src/Managers/PhysicsManager.h
#pragma once
#include "IManager.h"
#include <vector>
#include <glm/vec3.hpp>
#include "../Components/PhysicsComp.h"

class PhysicsManager : public IManager {
public:
    PhysicsManager();
    ~PhysicsManager() override = default;

    void Init() override {}
    void Update(float dt) override;
    void Shutdown() override {}

    void Register(PhysicsComp* c);
    void Unregister(PhysicsComp* c);

    // Debug / tweak
    int lastCollisionCount = 0;
    bool pause = false;
    float restitution = 0.0f; // [0,1]

private:
    float step = 1.0f / 120.0f;
    float accumulator = 0.0f;

    std::vector<PhysicsComp*> comps;

    // collision helpers
    static bool AABBOverlap(const AABB& a, const AABB& b);
    static bool SweptAABB(PhysicsComp* moving, PhysicsComp* other, float dt, float &tHit, glm::vec3& outNormal);
};

#endif //GEODASH3D_PHYSICSMANAGER_H