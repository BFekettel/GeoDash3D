//
// Created by Ben on 14/09/2025.
//

#ifndef PROJECT_LIGHTCOMP_H
#define PROJECT_LIGHTCOMP_H

#include "../Components/Component.h"
#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>

class LightComp : public Component {
public:
    LightComp();
    ~LightComp() = default;

    float intensity = 1.0f;             // overall brightness
    glm::vec3 color = glm::vec3(1.0f);

    // Spot/Directional parameters
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

    // Cutoff angles (for spotlights, in radians)
    float cutOff      = glm::cos(glm::radians(12.5f)); // inner cutoff
    float outerCutOff = glm::cos(glm::radians(17.5f)); // outer cutoff

    // Radius (for point & spot lights)
    float radius = 10.0f;               // default reach of point/spot
};

#endif //PROJECT_LIGHTCOMP_H
