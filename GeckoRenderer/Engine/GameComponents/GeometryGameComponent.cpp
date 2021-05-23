#include "GeometryGameComponent.h"

#include "../Game.h"

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
  auto view = game->Camera->getView();
  auto projection = game->Camera->getProjection();
  
  Geometry->Draw(DirectX::SimpleMath::Matrix::Identity, view, projection);
}

void GeometryGameComponent::DestroyResources()
{
}
