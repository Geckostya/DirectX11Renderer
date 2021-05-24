#pragma once
#include "SimpleMath.h"
#include "IHierarchical.h"

class TransformComponent
{
    DirectX::SimpleMath::Matrix tm;
    DirectX::SimpleMath::Matrix rootTm;
    IHierarchical* owner;

public:
    TransformComponent(IHierarchical* owner);
    ~TransformComponent() = default;

    DirectX::SimpleMath::Matrix GetTm() const;
    DirectX::SimpleMath::Matrix GetRootTm() const;
    void SetTm(DirectX::SimpleMath::Matrix&& matrix);
    void UpdateRootTm();
};
