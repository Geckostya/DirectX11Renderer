#include "GridGameComponent.h"

#include "BufferHelpers.h"


GridGameComponent::GridGameComponent(Game* game, int cellsCount, float cellSize)
    : GameComponent(game), cellsCount(cellsCount), cellSize(cellSize)
{
}

void GridGameComponent::Initialize()
{
    vertexShader = new VertexShader(L"Lines.fx");
    vertexShader->initialize(game->Device);

    D3D_SHADER_MACRO Shader_Macros[] = {"LINES_COLOR", "float4(1.0f, 1.0f, 1.0f, 1.0f)", nullptr, nullptr};

    pixelShader = new PixelShader(L"Lines.fx", "PSMain", Shader_Macros);
    pixelShader->initialize(game->Device);

    D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        D3D11_INPUT_ELEMENT_DESC{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    game->Device->CreateInputLayout(
        inputElements,
        sizeof(inputElements) / sizeof(D3D11_INPUT_ELEMENT_DESC),
        vertexShader->Bytecode()->GetBufferPointer(),
        vertexShader->Bytecode()->GetBufferSize(),
        &layout);

    using namespace DirectX::SimpleMath;

    const int n = (cellsCount + 1) * 4;
    Vector3* points = new Vector3[n];

    const float offset = cellsCount * cellSize / 2;
    for (int i = 0; i <= cellsCount; i++)
    {
        float iOffset = i * cellSize;
        points[i * 4] = Vector3::Right * (-offset + iOffset) + Vector3::Forward * -offset;
        points[i * 4 + 1] = Vector3::Right * (-offset + iOffset) + Vector3::Forward * offset;

        points[i * 4 + 2] = Vector3::Forward * (-offset + iOffset) + Vector3::Right * -offset;
        points[i * 4 + 3] = Vector3::Forward * (-offset + iOffset) + Vector3::Right * offset;
    }

    int* indices = new int[n];
    for (int i = 0; i < n; i++)
    {
        indices[i] = i;
    }

    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(Vector3) * n;

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
    indexBufDesc.ByteWidth = sizeof(int) * n;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    game->Device->CreateBuffer(&indexBufDesc, &indexData, &indicesBuffer);

    mvpBuffer.Create(game->Device.Get());

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    HRESULT res = game->Device->CreateRasterizerState(&rastDesc, &rastState);  ZCHECK(res);

    delete points;
    delete indices;
}

void GridGameComponent::Update(float deltaTime)
{
    mvpBuffer.SetData(game->Context, game->Camera->Get());
}

void GridGameComponent::Draw(float deltaTime)
{
    ID3D11Buffer* vBuffers[] = {verticesBuffer};
    UINT strides[] = {3 * 4};
    UINT offsets[] = {0};

    game->Context->RSSetState(rastState);

    game->Context->IASetInputLayout(layout);
    game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    game->Context->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->Context->IASetVertexBuffers(0, sizeof(vBuffers) / sizeof(ID3D11Buffer*), vBuffers, strides, offsets);
    game->Context->VSSetShader(vertexShader->Get(), nullptr, 0);
    game->Context->PSSetShader(pixelShader->Get(), nullptr, 0);

    ID3D11Buffer* pp_constant_buffers = mvpBuffer.GetBuffer();
    game->Context->VSSetConstantBuffers(0, 1, &pp_constant_buffers);

    const int n = (cellsCount + 1) * 4;
    game->DebugAnnotation->BeginEvent(L"Grid Draw Event");
    game->Context->DrawIndexed(n, 0, 0);
    game->DebugAnnotation->EndEvent();
}

void GridGameComponent::DestroyResources()
{
    if (vertexShader) delete vertexShader;
    if (pixelShader) delete pixelShader;
}
