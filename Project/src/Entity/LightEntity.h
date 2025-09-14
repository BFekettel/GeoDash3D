//
// Created by Ben on 14/09/2025.
//

#ifndef PROJECT_LIGHTENTITY_H
#define PROJECT_LIGHTENTITY_H
#include "Entity.h"
#include "../Components/RadiusLightComp.h"

class LightEntity: public Entity {
    public:
    LightEntity();
    ~LightEntity() = default;

    RadiusLightComp Light;

    glm::vec3 getColor();
    void setColor(glm::vec3 color);

    float getIntensity();
    void setIntensity(float intensity);

    float getGradient();
    void setGradient(float gradient);

    float getRadius();
    void setRadius(float radius);
};


#endif //PROJECT_LIGHTENTITY_H