//
// Created by Ben on 14/09/2025.
//

#include "LightEntity.h"

LightEntity::LightEntity() {
    // Default values
    setIntensity(1.0f);
    setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    setCutOff(glm::cos(glm::radians(12.5f)));
    setOuterCutOff(glm::cos(glm::radians(17.5f)));

    // Model for visualizing the light in the scene
    StaticMesh.loadModel("../content/models/lightshape.obj");
}

void LightEntity::setDirection(float yawDeg, float pitchDeg) {
    float yaw   = glm::radians(yawDeg);
    float pitch = glm::radians(pitchDeg);

    glm::vec3 dir;
    dir.x = cos(pitch) * cos(yaw);
    dir.y = sin(pitch);
    dir.z = cos(pitch) * sin(yaw);

    Light.direction = glm::normalize(dir);
}


