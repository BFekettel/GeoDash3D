//
// Created by Ben on 14/09/2025.
//

#include "LightEntity.h"

LightEntity::LightEntity() {
    setIntensity(1.0f);
    setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    setGradient(0.5f);
    setRadius(50.0f);
}

glm::vec3 LightEntity::getColor() {
    return Light.color;
}

void LightEntity::setColor(glm::vec3 color) {
    Light.color = color;
}

float LightEntity::getIntensity() {
    return Light.intensity;
}

void LightEntity::setIntensity(float intensity) {
    Light.intensity = intensity;
}

float LightEntity::getGradient() {
    return Light.gradient;
}

void LightEntity::setGradient(float gradient) {
    Light.gradient = gradient;
}

float LightEntity::getRadius() {
    return Light.radius;
}

void LightEntity::setRadius(float radius) {
    Light.radius = radius;
}
