//
// Created by chee7 on 9/27/25.
//

#ifndef GEODASH3D_IMANAGER_H
#define GEODASH3D_IMANAGER_H
class IManager {
public:
    virtual ~IManager() = default;
    virtual void Init() {}
    virtual void Update(float) {}
    virtual void Shutdown() {}
};

#endif //GEODASH3D_IMANAGER_H