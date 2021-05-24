#include "GeometryGameComponent.h"

#include "../Game.h"

GeometryGameComponent::GeometryGameComponent(Game* game, std::unique_ptr<DirectX::GeometricPrimitive>&& geometry,
  IHierarchical* parent)
  : GameComponent(game),
    Geometry(std::move(geometry)),
    tmComponent(new TransformComponent(this)),
    hierarchyComponent(new HierarchyComponent(this))
{
    if (parent) parent->GetHierarchyComponent()->AddChild(this);
}

GeometryGameComponent::~GeometryGameComponent()
{
  delete tmComponent;
  delete hierarchyComponent;
  GameComponent::~GameComponent();
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
  const auto view = game->Camera->getView();
  const auto projection= game->Camera->getProjection();
  
  Geometry->Draw(tmComponent->GetRootTm(), view, projection);
}

void GeometryGameComponent::DestroyResources()
{
}

TransformComponent* GeometryGameComponent::GetTransformComponent() const
{
  return tmComponent;
}

HierarchyComponent* GeometryGameComponent::GetHierarchyComponent() const
{
  return hierarchyComponent;
}
