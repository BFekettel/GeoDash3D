#pragma once
#include <glm/vec3.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

// returns true if a and b overlap (non-empty intersection).
// Touching exactly on a face will be considered overlapping if preferred
// adjust comparison operators  (<= vs <)
inline bool AABBOverlap(const AABB& a, const AABB& b) {
    // Overlap on X, Y and Z axes
    if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
    if (a.max.z < b.min.z || a.min.z > b.max.z) return false;
    return true;
}
