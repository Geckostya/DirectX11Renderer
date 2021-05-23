#include "FPSCameraController.h"

#include "../Game.h"


FPSCameraController::FPSCameraController(Game* game, CameraMatrix* camera): CameraController(camera), game(game)
{
    lastMouseState = game->inputDevice->getMouse().GetState();
    cameraPosition = camera->GetPosition();
}

void FPSCameraController::Update(float deltaTime)
{
    UpdatePitchYaw(deltaTime);
    UpdatePosition(deltaTime);
    camera->UpdateProjectionMatrix();
}

void FPSCameraController::UpdatePitchYaw(float deltaTime)
{
    DirectX::Mouse::State currentMouseState = game->inputDevice->getMouse().GetState();

    if (currentMouseState.leftButton)
    {
        int dx = currentMouseState.x - lastMouseState.x; 
        int dy = currentMouseState.y - lastMouseState.y;

        yaw -= dx * mouseSensitivity * deltaTime;
        pitch -= dy * mouseSensitivity * deltaTime;
    }
        
    if (currentMouseState.scrollWheelValue > 0 && cameraVelocity + deltaTime <= 10.0f) cameraVelocity += 10 * deltaTime;
    if (currentMouseState.scrollWheelValue < 0 && cameraVelocity - deltaTime >= 1.0f) cameraVelocity -= 10 * deltaTime;
    
    lastMouseState = std::move(currentMouseState);
}


void FPSCameraController::UpdatePosition(float deltaTime)
{
    using namespace DirectX::SimpleMath;
    auto& input = game->inputDevice;

    const Matrix rotMat = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);
    
    Vector3 offset = Vector3::Zero;
    if (input->IsKeyDown(DirectX::Keyboard::D)) offset += Vector3::Right;
    if (input->IsKeyDown(DirectX::Keyboard::A)) offset += Vector3::Left;
    if (input->IsKeyDown(DirectX::Keyboard::W)) offset += Vector3::Forward;
    if (input->IsKeyDown(DirectX::Keyboard::S)) offset += Vector3::Backward;
    if (input->IsKeyDown(DirectX::Keyboard::Space)) offset += Vector3::Up;
    if (input->IsKeyDown(DirectX::Keyboard::LeftShift)) offset += Vector3::Down;

    offset = -rotMat.Forward() * offset.z + rotMat.Right() * offset.x + Vector3::Up * offset.y;
    offset.Normalize();

    cameraPosition = cameraPosition + offset * cameraVelocity * deltaTime;

    
    auto newView = Matrix::CreateLookAt(cameraPosition, cameraPosition + rotMat.Forward(), rotMat.Up());
    camera->SetView(newView);
}
