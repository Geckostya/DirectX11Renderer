#pragma once
#include <corecrt_math_defines.h>

#include "SimpleMath.h"

class Game;

class CameraMatrix
{
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;
	Game* game;
	float fov;

public:
	CameraMatrix(Game* game, float fov = static_cast<float>(M_PI) / 2.0f);

	void SetFow(float fov);

	void SetView(DirectX::SimpleMath::Matrix view);
	DirectX::SimpleMath::Matrix Get() const;

	const DirectX::SimpleMath::Matrix& getView() const; 
	const DirectX::SimpleMath::Matrix& getProjection() const; 
	
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetForwardVector() const;

	void UpdateProjectionMatrix();
};

