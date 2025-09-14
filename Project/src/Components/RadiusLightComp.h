//
// Created by Ben on 14/09/2025.
//

#ifndef PROJECT_RADIUSLIGHT_H
#define PROJECT_RADIUSLIGHT_H
#include "../Components/Component.h"
#include "glm/vec3.hpp"

class RadiusLightComp: public Component {
    public:
    RadiusLightComp();
    ~RadiusLightComp() = default;

    float radius; //how far light effects
    float gradient; //gradient of light based on radius
    float intensity;

    glm::vec3 color;


};


#endif //PROJECT_RADIUSLIGHT_H