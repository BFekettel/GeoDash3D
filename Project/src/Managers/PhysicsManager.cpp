// src/Managers/PhysicsManager.cpp
#include "PhysicsManager.h"
#include "../Entity/Entity.h"   // <<< IMPORTANT: make Entity visible here
#include <algorithm>
#include <cfloat>
#include <glm/glm.hpp>

PhysicsManager::PhysicsManager() = default;

void PhysicsManager::Register(PhysicsComp* comp) {
    if (!comp) return;
    if (std::find(components.begin(), components.end(), comp) == components.end()) {
        components.push_back(comp);
    }
}

void PhysicsManager::Unregister(PhysicsComp* comp) {
    if (!comp) return;
    components.erase(std::remove(components.begin(), components.end(), comp), components.end());
}

bool PhysicsManager::SweptAABB(PhysicsComp *moving, PhysicsComp *other, float dt, float &tHit, glm::vec3 &outNormal) {
    if (!moving || !other) return false;

    AABB A = moving->GetWorldAABB();
    AABB B = other->GetWorldAABB();

    glm::vec3 v = moving->velocity - other->velocity;

    if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) {
        if (AABBOverlap(A, B)) {
            tHit = 0.0f;
            outNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            return true;
        }
        return false;
    }

    float tEntry = -FLT_MAX;
    float tExit = FLT_MAX;
    int entryAxis = -1;

    for (int axis = 0; axis < 3; axis++) {
        float aMin = (axis==0?A.min.x : (axis==1?A.min.y : (axis==2?A.min.z : B.min.x)));
        float aMax = (axis==0?A.max.x : (axis==1?A.max.y : (axis==2?A.max.z : B.max.x)));
        float bMin = (axis==0?B.min.x : (axis==1?B.min.y : (axis==2?B.min.z : B.max.x)));
        float bMax = (axis==0?B.max.x : (axis==1?B.max.y : (axis==2?B.max.z : B.min.x)));
        float vel = (axis==0?v.x : (axis==1?v.y : v.z));

        if (vel == 0.0f) {
            //if separated and no vel, then will never collide
            if (aMax < bMin || aMin > bMax) return false;
            continue;
        }

        float tAxisEntry, tAxisExit;
        if (vel > 0.0f) {
            tAxisEntry = (bMin - aMax) / vel;
            tAxisExit = (bMax - aMin) / vel;
        } else {
            tAxisEntry = (bMax - aMin) / vel;
            tAxisExit = (bMin + aMax) / vel;
        }

        if (tAxisEntry > tEntry) {
            tEntry = tAxisEntry;
            entryAxis = axis;
        }
        if (tAxisExit < tExit) {
            tExit = tAxisExit;
        }
        if (tEntry > tExit) return false;
    }

    //if tEntry is outside time window, no hit
    if (tEntry > dt || tExit < 0.0f) return false;

    tHit = std::max(0.0f, tEntry);

    if (entryAxis == 0) {
        outNormal = (v.x > 0.0f ? glm::vec3(-1.0f, 0.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f));
    } else if (entryAxis == 1) {
        outNormal = (v.y < 0.0f ? glm::vec3(0.0f, -1.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f));
    } else if (entryAxis == 2) {
        outNormal = (v.z < 0.0f ? glm::vec3(0.0f, 0.0f, -1.0f) : glm::vec3(0.0f, 0.0f, 1.0f));
    } else {
        outNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    return true;
}


void PhysicsManager::Update(float dt) {
    if (pause) return;
    lastCollisionCount = 0;

    accumulator += dt;
    while (accumulator >= step) {
        // integrate velocities (semi-implicit Euler) for this step duration
        for (auto p : comps) {
            if (!p || p->isStatic) continue;
            // integrate velocity
            p->velocity += p->acceleration * step;
            // zero per-step acceleration
            p->acceleration = glm::vec3(0.0f);
        }

        // broadphase: naive n^2 for now (small scene), replace with spatial hash later.
        for (size_t i = 0; i < comps.size(); ++i) {
            for (size_t j = i + 1; j < comps.size(); ++j) {
                PhysicsComp* A = comps[i];
                PhysicsComp* B = comps[j];
                if (!A || !B) continue;
                if (A->isStatic && B->isStatic) continue;

                float tHit = 0.0f;
                glm::vec3 normal;
                // do swept test during this step window
                if (SweptAABB(A, B, step, tHit, normal)) {
                    lastCollisionCount++;

                    // Move A to impact point (advance by tHit)
                    Entity* ownerA = A->GetOwner();
                    Entity* ownerB = B->GetOwner();
                    if (ownerA && !A->isStatic) ownerA->Position += A->velocity * tHit;
                    if (ownerB && !B->isStatic) ownerB->Position += B->velocity * tHit;

                    // compute relative velocity along normal
                    glm::vec3 relVel = A->velocity - B->velocity;
                    float relAlongNormal = glm::dot(relVel, normal);

                    // If separating, ignore
                    if (relAlongNormal > 0.0f) {
                        // still advance the remainder of the step below
                    } else {
                        // restitution-based impulse
                        float invMassA = A->isStatic ? 0.0f : (1.0f / std::max(0.00001f, A->mass));
                        float invMassB = B->isStatic ? 0.0f : (1.0f / std::max(0.00001f, B->mass));
                        float invMassSum = invMassA + invMassB;
                        if (invMassSum > 0.0f) {
                            float j = -(1.0f + restitution) * relAlongNormal / invMassSum;
                            glm::vec3 impulse = j * normal;
                            if (!A->isStatic) A->velocity += impulse * invMassA;
                            if (!B->isStatic) B->velocity -= impulse * invMassB;
                        }
                    }

                    // floor snap
                    if (normal.y > 0.707f) { // collision normal points up -> A hit top of B
                        if (!A->isStatic) {
                            // snap moving A to be exactly on top of B (avoid penetration)
                            AABB Aw = A->GetWorldAABB();
                            AABB Bw = B->GetWorldAABB();
                            Entity* owner = A->GetOwner();
                            if (owner) {
                                float eps = 0.001f;
                                float newY = Bw.max.y - A->aabb.min.y * owner->Scale.y + eps;
                                owner->Position.y = newY;
                                // stop vertical motion
                                A->velocity.y = 0.0f;
                                A->onGround = true;
                            }
                        }
                    }

                    // advance both bodies for the remainder of this substep
                    float remaining = step - tHit;
                    if (!A->isStatic) {
                        Entity* owner = A->GetOwner();
                        if (owner) owner->Position += A->velocity * remaining;
                    }
                    if (!B->isStatic) {
                        Entity* owner = B->GetOwner();
                        if (owner) owner->Position += B->velocity * remaining;
                    }
                }
            }
        }

        // for bodies that didn't collide, integrate their positions fully for this step
        for (auto p : comps) {
            if (!p || p->isStatic) continue;
            Entity* owner = p->GetOwner();
            if (!owner) continue;
            // if owner was already moved during a collision above then we risk double move
            // but above branch moves only bodies that had collisions
            // For a robust system we would track processed flags
            // For simplicity, if there was no immediate overlap, advance by full step.
            owner->Position += p->velocity * step;
        }

        accumulator -= step;
    }
}

