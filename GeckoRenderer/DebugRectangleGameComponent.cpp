#include "DebugRectangleGameComponent.h"

#include "DebugRectangleGameComponent.h"
#include "pch.h"
#include "Game.h"

void DebugRectangleGameComponent::Initialize()
{
	ID3DBlob* vertexBC;
	ID3DBlob* errorVertexCode;
	HRESULT res = D3DCompileFromFile(L"MiniTri.fx",
	                             nullptr /*macros*/,
	                             nullptr /*include*/,
	                             "VSMain",
	                             "vs_5_0",
	                             D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	                             0,
	                             &vertexBC,
	                             &errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(game->Display->handle, L"MiniTri.fx", L"Missing Shader File", MB_OK);
		}

		exit(1);
	}


	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* pixelBC;
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"MiniTri.fx", Shader_Macros , nullptr , "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC, &errorPixelCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(game->Display->handle, L"MiniTri.fx", L"Missing Shader File", MB_OK);
		}

		exit(1);
	}

	auto& device = game->Device;

	device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);

	device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	
	using namespace DirectX::SimpleMath;
	
	Vector4 points[8] = {
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	};


	int indeces[] = { 0,1,2, 1,0,3 };

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

	device->CreateBuffer(&vertexBufDesc, &vertexData, &verticesBuffer);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, &indicesBuffer);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = device->CreateRasterizerState(&rastDesc, &rastState); ZCHECK(res);
}

DebugRectangleGameComponent::DebugRectangleGameComponent(Game* game) : GameComponent(game)
{
}

void DebugRectangleGameComponent::Reload()
{
}

void DebugRectangleGameComponent::Update(float deltaTime)
{
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
	game->Context->VSSetShader(vertexShader, nullptr, 0);
	game->Context->PSSetShader(pixelShader, nullptr, 0);
	
	game->DebugAnnotation->BeginEvent(L"BeginDraw");
	game->Context->DrawIndexed(6, 0, 0);
	game->DebugAnnotation->EndEvent();
}

void DebugRectangleGameComponent::DestroyResources()
{
	vertexShader->Release();
	pixelShader->Release();
}
