//
// Created by chee7 on 9/27/25.
//

#include "../src/Physics/AABB.h"
#include <gtest/gtest.h>

// Utility to make an AABB from center + half extents
static AABB MakeAABB(float cx, float cy, float cz, float hx, float hy, float hz) {
    AABB a;
    a.min = glm::vec3(cx - hx, cy - hy, cz - hz);
    a.max = glm::vec3(cx + hx, cy + hy, cz + hz);
    return a;
}

TEST(AABBOverlapTests, SeparateBoxes) {
    auto a = MakeAABB(0,0,0, 1,1,1);
    auto b = MakeAABB(3,0,0, 1,1,1); // separated on X
    EXPECT_FALSE(AABBOverlap(a,b));
}

TEST(AABBOverlapTests, TouchingEdges) {
    auto a = MakeAABB(0,0,0, 1,1,1);
    auto b = MakeAABB(2,0,0, 1,1,1); // touches on x = 1 face
    // If AABBOverlap uses < checks, touching is overlap. For strict non-touch use <= and EXPECT_FALSE.
    EXPECT_TRUE(AABBOverlap(a,b));
}

TEST(AABBOverlapTests, OneInsideAnother) {
    auto a = MakeAABB(0,0,0, 3,3,3);
    auto b = MakeAABB(0,0,0, 1,1,1);
    EXPECT_TRUE(AABBOverlap(a,b));
}

TEST(AABBOverlapTests, CornerTouch) {
    auto a = MakeAABB(0,0,0, 1,1,1);
    auto b = MakeAABB(2,2,2, 1,1,1); // touches at a single corner
    EXPECT_TRUE(AABBOverlap(a,b));
}
