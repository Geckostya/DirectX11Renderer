#pragma once
#include "GameComponent.h"
#include "GeometricPrimitive.h"

class GeometryGameComponent
	: public GameComponent
{
private:
	std::unique_ptr<DirectX::GeometricPrimitive> Geometry;
public:
	explicit GeometryGameComponent(Game* game, std::unique_ptr<DirectX::GeometricPrimitive>&& geometry);

	void Initialize() override;
	void Reload() override;
	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;
	void DestroyResources() override;
};

