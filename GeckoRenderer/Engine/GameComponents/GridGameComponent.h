#pragma once
#include "BufferHelpers.h"
#include "GameComponent.h"
#include "../pch.h"
#include "../Graphics/Shader.h"

class GridGameComponent
    : public GameComponent
{
    VertexShader* vertexShader = nullptr;
    PixelShader* pixelShader = nullptr;
    ID3D11InputLayout* layout = nullptr;
    ID3D11RasterizerState* rastState = nullptr;
    ID3D11Buffer* verticesBuffer = nullptr;
    ID3D11Buffer* indicesBuffer = nullptr;
    
    DirectX::ConstantBuffer<DirectX::SimpleMath::Matrix> mvpBuffer;

    int cellsCount;
    float cellSize;

public:
    explicit GridGameComponent(Game* game, int cellsCount, float cellSize);

    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw(float deltaTime) override;
    void DestroyResources() override;
};
