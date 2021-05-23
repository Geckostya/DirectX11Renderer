#pragma once
#include "CameraController.h"
#include "Mouse.h"

class FPSCameraController
    : public CameraController
{
    Game* game;
    float yaw = 0;
    float pitch = 0;

    float mouseSensitivity = 0.3f;
    float cameraVelocity = 2.0f;

    DirectX::SimpleMath::Vector3 cameraPosition; 

    DirectX::Mouse::State lastMouseState;
public:
    FPSCameraController(Game* game, CameraMatrix* camera);
    ~FPSCameraController() override = default;
    void Update(float deltaTime) override;

private:
    void UpdatePosition(float deltaTime);
    void UpdatePitchYaw(float deltaTime);
};
