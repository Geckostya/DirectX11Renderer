#pragma once

class GameComponent;
class TransformComponent;
class HierarchyComponent;

class IHierarchical
{
public:
    virtual ~IHierarchical() = default;
    virtual TransformComponent* GetTransformComponent() const = 0;
    virtual HierarchyComponent* GetHierarchyComponent() const = 0;
};
