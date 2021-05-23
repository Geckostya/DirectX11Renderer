#include "DebugRectangleGameComponent.h"

#include "../Game.h"

void DebugRectangleGameComponent::Initialize()
{
	vertexShader = new VertexShader(L"MiniTri.fx");
	vertexShader->initialize(game->Device);

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	pixelShader = new PixelShader(L"MiniTri.fx", "PSMain", Shader_Macros);
	pixelShader->initialize(game->Device);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShader->Bytecode()->GetBufferPointer(),
		vertexShader->Bytecode()->GetBufferSize(),
		&layout);
	
	using namespace DirectX::SimpleMath;
	
	Vector4 points[8] = {
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	};


	int indices[] = { 0,1,2, 1,0,3 };

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&vertexBufDesc, &vertexData, &verticesBuffer);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indices);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&indexBufDesc, &indexData, &indicesBuffer);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	HRESULT res = game->Device->CreateRasterizerState(&rastDesc, &rastState); ZCHECK(res);
}

void DebugRectangleGameComponent::Draw(float deltaTime)
{
	ID3D11Buffer* vBuffers[] = { verticesBuffer };
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	game->Context->RSSetState(rastState);
	
	game->Context->IASetInputLayout(layout);
	game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->Context->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->Context->IASetVertexBuffers(0, sizeof(vBuffers) / sizeof(ID3D11Buffer*), vBuffers, strides, offsets);
	game->Context->VSSetShader(vertexShader->Get(), nullptr, 0);
	game->Context->PSSetShader(pixelShader->Get(), nullptr, 0);
	
	game->DebugAnnotation->BeginEvent(L"BeginDraw");
	game->Context->DrawIndexed(6, 0, 0);
	game->DebugAnnotation->EndEvent();
}

void DebugRectangleGameComponent::DestroyResources()
{
	if (vertexShader) delete vertexShader;
	if (pixelShader) delete pixelShader;
}
