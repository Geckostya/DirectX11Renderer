#include "SolarSystemGame.h"

#include "Engine/Camera/FPSCameraController.h"
#include "Engine/GameComponents/GeometryGameComponent.h"
#include "Engine/GameComponents/GridGameComponent.h"

void SolarSystemGame::Initialize()
{
	Game::Initialize();
	CameraController = new FPSCameraController(this, Camera);

	GameComponents.push_back(new GridGameComponent(this, 10, 1.0f));
	GameComponents.push_back(new GeometryGameComponent(this, 
		DirectX::GeometricPrimitive::CreateBox(Context, DirectX::SimpleMath::Vector3(1, 1, 1)))
	);
}

void SolarSystemGame::Update(float deltaTime)
{
	CameraController->Update(deltaTime);
	Game::Update(deltaTime);
}


