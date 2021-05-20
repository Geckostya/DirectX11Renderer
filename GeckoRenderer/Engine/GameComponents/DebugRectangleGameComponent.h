#pragma once
#include "GameComponent.h"
#include "../pch.h"

class DebugRectangleGameComponent :
    public GameComponent
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11RasterizerState* rastState;
	ID3D11Buffer* verticesBuffer;
	ID3D11Buffer* indicesBuffer;

public:	
	DebugRectangleGameComponent(Game* game);
	
	virtual void Initialize() override;
	virtual void Reload() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(float deltaTime) override;
	virtual void DestroyResources() override;
};

