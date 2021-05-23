#pragma once
#include "GameComponent.h"
#include "../pch.h"
#include "../Graphics/Shader.h"

class DebugRectangleGameComponent :
    public GameComponent
{
	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* layout = nullptr;
	ID3D11RasterizerState* rastState = nullptr;
	ID3D11Buffer* verticesBuffer = nullptr;
	ID3D11Buffer* indicesBuffer = nullptr;

public:	
	DebugRectangleGameComponent(Game* game);
	
	virtual void Initialize() override;
	virtual void Draw(float deltaTime) override;
	virtual void DestroyResources() override;
};

