#include "SolarSystemGame.h"

#include "Engine/GameComponents/DebugRectangleGameComponent.h"
#include "Engine/GameComponents/GeometryGameComponent.h"

void SolarSystemGame::Initialize()
{
	GameComponents.push_back(new DebugRectangleGameComponent(this));
	GameComponents.push_back(new GeometryGameComponent(this, 
		DirectX::GeometricPrimitive::CreateBox(Context, DirectX::SimpleMath::Vector3(1, 1, 1)))
	);
}
