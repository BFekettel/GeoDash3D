//
// Created by chee7 on 9/26/25.
//

#ifndef GEODASH3D_LEVELMANAGER_H
#define GEODASH3D_LEVELMANAGER_H

#pragma once
#include "IManager.h"
#include <string>
#include <vector>
#include <memory>

class Entity; // forward

class LevelManager : public IManager {
public:
    void Init() override {}
    void Shutdown() override {}

    bool LoadLevel(const std::string& path);
    void UnloadLevel();
    void PlayLevel();
    void RestartLevel();

    // spawn/despawn
    Entity* SpawnPrefab(const std::string& prefabName, const glm::vec3& pos);
    void Despawn(Entity* e);
};


#endif //GEODASH3D_LEVELMANAGER_H