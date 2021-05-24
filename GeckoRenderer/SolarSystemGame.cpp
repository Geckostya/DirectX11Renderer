#include "SolarSystemGame.h"

#include "Engine/Camera/FPSCameraController.h"
#include "Engine/GameComponents/GeometryGameComponent.h"
#include "Engine/GameComponents/GridGameComponent.h"

void SolarSystemGame::Initialize()
{
	Game::Initialize();
	CameraController = new FPSCameraController(this, Camera);

	using namespace DirectX::SimpleMath;
	
	GameComponents.push_back(new GridGameComponent(this, 10, 1.0f));
	cube1 = new GeometryGameComponent(
		this, DirectX::GeometricPrimitive::CreateBox(Context, Vector3(1, 1, 1))
	);
	cube1->GetTransformComponent()->SetTm(Matrix::CreateTranslation(Vector3(0, 0.5, 0)));
	cube2 = new GeometryGameComponent(
		this, DirectX::GeometricPrimitive::CreateBox(Context, Vector3(0.5, 0.5, 0.5)), cube1
	);
	cube2->GetTransformComponent()->SetTm(Matrix::CreateTranslation(Vector3(0, 0, 2)));
	GameComponents.push_back(cube1);
	GameComponents.push_back(cube2);
}

void SolarSystemGame::Update(float deltaTime)
{
	CameraController->Update(deltaTime);

	using namespace DirectX::SimpleMath;
	float t;
	cube2->GetTransformComponent()->SetTm(
		cube2->GetTransformComponent()->GetTm() * Matrix::CreateRotationY(2 * M_PI * modf(deltaTime / 8, &t))
	);
	cube1->GetTransformComponent()->SetTm(
		Matrix::CreateTranslation(Vector3(2 * sin(getTime()), 0.5, 0))
	);

	Game::Update(deltaTime);
}


