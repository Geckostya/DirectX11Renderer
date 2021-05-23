#pragma once
#include "CameraMatrix.h"

class CameraController
{
protected:
    CameraMatrix* camera;
public:
    CameraController(CameraMatrix* camera) : camera(camera) {}
    virtual ~CameraController() = default;

    virtual void Update(float deltaTime) {}
};

