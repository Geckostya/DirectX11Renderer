#include "HierarchyComponent.h"

#include "TransformComponent.h"

HierarchyComponent::HierarchyComponent(IHierarchical* owner)
    : owner(owner)
{
}

HierarchyComponent::~HierarchyComponent()
{
    if (children)
    {
        while (children->size() > 0)
        {
            const auto child = children->back();
            child->GetHierarchyComponent()->DetachFromParent();
            if (const auto parent = GetParent()) parent->GetHierarchyComponent()->AddChild(child);
        }
    }
    DetachFromParent();
}

IHierarchical* HierarchyComponent::GetOwner() const
{
    return owner;
}

IHierarchical* HierarchyComponent::GetParent() const
{
    return ownerParent;
}

const std::vector<IHierarchical*>* HierarchyComponent::GetChildren() const
{
    return children;
}

IHierarchical* HierarchyComponent::AddChild(IHierarchical* child)
{
    if (!children) children = new std::vector<IHierarchical*>;
    children->push_back(child);
    IHierarchical** oldParent = &(child->GetHierarchyComponent()->ownerParent);
    if (*oldParent) child->GetHierarchyComponent()->DetachFromParent();
    *oldParent = owner;
    child->GetTransformComponent()->UpdateRootTm();
    return child;
}

void HierarchyComponent::DetachFromParent()
{
    if (const auto parent = GetParent())
    {
        if (auto parentChildren = parent->GetHierarchyComponent()->children)
        {
           parentChildren->erase(
               std::remove(parentChildren->begin(), parentChildren->end(), owner),
               parentChildren->end()
           );
        }
        ownerParent = nullptr;
        GetOwner()->GetTransformComponent()->UpdateRootTm();
    }
}
