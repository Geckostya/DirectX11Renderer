#include "GeometryGameComponent.h"

GeometryGameComponent::GeometryGameComponent(Game* game, std::unique_ptr<DirectX::GeometricPrimitive>&& geometry)
: GameComponent(game),
  Geometry(std::move(geometry))
{
}

void GeometryGameComponent::Initialize()
{
}

void GeometryGameComponent::Reload()
{
}

void GeometryGameComponent::Update(float deltaTime)
{
}

void GeometryGameComponent::Draw(float deltaTime)
{
}

void GeometryGameComponent::DestroyResources()
{
}
