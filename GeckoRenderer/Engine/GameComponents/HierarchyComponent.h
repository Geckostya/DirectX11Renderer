#pragma once
#include <vector>

#include "IHierarchical.h"

class GameComponent;

class HierarchyComponent
{
    std::vector<IHierarchical*>* children = nullptr;
    IHierarchical* owner = nullptr;
    IHierarchical* ownerParent = nullptr;
public:
    HierarchyComponent(IHierarchical* owner);
    ~HierarchyComponent();
    
    IHierarchical* GetOwner() const;
    IHierarchical* GetParent() const;
    const std::vector<IHierarchical*>* GetChildren() const;
    IHierarchical* AddChild(IHierarchical* child);
    void DetachFromParent();
};
