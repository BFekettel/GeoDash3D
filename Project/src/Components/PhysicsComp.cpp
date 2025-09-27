//
// Created by chee7 on 9/26/25.
//

#include "PhysicsComp.h"
#include "../Managers/ManagerRegistry.h"
#include "../Managers/PhysicsManager.h"
#include "../Entity/Entity.h"

void PhysicsComp::init() {
    auto pm = ManagerRegistry::Instance().Get<PhysicsManager>();
    if (pm) pm->Register(this);
}

void PhysicsComp::destroy() {
    auto pm = ManagerRegistry::Instance().Get<PhysicsManager>();
    if (pm) pm->Unregister(this);
}

AABB PhysicsComp::GetWorldAABB() const {
    AABB w;
    Entity* owner = GetOwner();
    if (!owner) {
        w.min = aabb.min;
        w.max = aabb.max;
        return w;
    }

    glm::vec3 scaledMax = glm::vec3(aabb.max.x * owner->Scale.x,
                                    aabb.max.y * owner->Scale.y,
                                    aabb.max.z * owner->Scale.z);
    glm::vec3 scaledMin = glm::vec3(aabb.min.x * owner->Scale.x,
                                    aabb.min.y * owner->Scale.y,
                                    aabb.min.z * owner->Scale.z);

    w.min = owner->Position + scaledMin;
    w.max = owner->Position + scaledMax;
    return w;
}
