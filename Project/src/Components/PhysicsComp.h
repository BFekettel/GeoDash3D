

#include <glm/vec3.hpp>
#include "Component.h"
#include "../Managers/ManagerRegistry.h"
#include "../Managers/PhysicsManager.h"
#include <algorithm>

enum class ShapeType { AABB };

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

class PhysicsComp : public Component {
public:
    PhysicsComp() : Component(nullptr) {}
    explicit PhysicsComp(Entity* owner) : Component(owner) {}

    float mass = 1.0f;
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    bool isStatic = false;
    bool onGround = false; // for platforming
    ShapeType shapeType = ShapeType::AABB;
    AABB aabb{glm::vec3(-0.5f), glm::vec3(0.5f)}; // local

    void init() override;
    void destroy() override;
    void tick(float dt) override { (void)dt; /* physics handled by manager */ }

    // compute world-space AABB using owner Position + Scale (caller must check owner)
    AABB GetWorldAABB() const;
};
