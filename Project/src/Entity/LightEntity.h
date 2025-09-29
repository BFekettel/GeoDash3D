#ifndef PROJECT_LIGHTENTITY_H
#define PROJECT_LIGHTENTITY_H

#include "Entity.h"
#include "../Components/LightComp.h"

enum class LightType {
    Global,   // directional
    Point,
    Spot
};

class LightEntity : public Entity {
public:
    LightEntity();
    ~LightEntity() = default;

    LightComp Light;

    // Type of light
    LightType Type = LightType::Point;

    // --- Getters / Setters ---
    glm::vec3 getColor() { return Light.color; }
    void setColor(const glm::vec3& color) { Light.color = color; }

    float getIntensity() { return Light.intensity; }
    void setIntensity(float intensity) { Light.intensity = intensity; }

    glm::vec3 getDirection() { return Light.direction; }
    void setDirection(float yawDeg, float pitchDeg);

    float getCutOff() { return Light.cutOff; }
    void setCutOff(float cutoff) { Light.cutOff = cutoff; }

    float getOuterCutOff() { return Light.outerCutOff; }
    void setOuterCutOff(float outerCutoff) { Light.outerCutOff = outerCutoff; }

    float getRadius() { return Light.radius; }
    void setRadius(float radius) { Light.radius = radius; }
};

#endif //PROJECT_LIGHTENTITY_H
