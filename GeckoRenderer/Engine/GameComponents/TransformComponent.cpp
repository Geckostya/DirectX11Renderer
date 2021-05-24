#include "TransformComponent.h"
#include "HierarchyComponent.h"

TransformComponent::TransformComponent(IHierarchical* owner) : owner(owner)
{
}

DirectX::SimpleMath::Matrix TransformComponent::GetTm() const
{
    return tm;
}

DirectX::SimpleMath::Matrix TransformComponent::GetRootTm() const
{
    return rootTm;
}

void TransformComponent::SetTm(DirectX::SimpleMath::Matrix&& matrix)
{
    tm = std::move(matrix);
    UpdateRootTm();
}

void TransformComponent::UpdateRootTm()
{
    const auto parent = owner->GetHierarchyComponent()->GetParent();
    rootTm = parent == nullptr ? tm : tm * parent->GetTransformComponent()->GetRootTm();
    if (const auto children = owner->GetHierarchyComponent()->GetChildren(); children != nullptr)
    {
        for (auto& child : *children)
        {
            child->GetTransformComponent()->UpdateRootTm();
        }
    }
}
