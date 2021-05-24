#pragma once
#include "GameComponent.h"
#include "GeometricPrimitive.h"
#include "IHierarchical.h"
#include "TransformComponent.h"
#include "HierarchyComponent.h"

class GeometryGameComponent
    : public GameComponent, public IHierarchical
{
    std::unique_ptr<DirectX::GeometricPrimitive> Geometry;
    TransformComponent* const tmComponent = nullptr;
    HierarchyComponent* const hierarchyComponent = nullptr;
public:
    explicit GeometryGameComponent(
        Game* game,
        std::unique_ptr<DirectX::GeometricPrimitive>&& geometry,
        IHierarchical* parent = nullptr
    );
    ~GeometryGameComponent() override;

    void Initialize() override;
    void Reload() override;
    void Update(float deltaTime) override;
    void Draw(float deltaTime) override;
    void DestroyResources() override;
    TransformComponent* GetTransformComponent() const override;
    HierarchyComponent* GetHierarchyComponent() const override;
};
