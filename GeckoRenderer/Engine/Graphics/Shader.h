#pragma once
#include "../pch.h"
#include "../Game.h"

template <typename ShaderType, const char *pTarget>
class Shader
{
	inline static std::wstring SHADER_FOLDER = L"Engine/Shaders/";
protected:

	ShaderType* shader;
	ID3DBlob* byteCodes;

	Shader(
		std::wstring shaderName, const char* entrypoint,
		D3D_SHADER_MACRO* macros = nullptr, ID3DInclude* includes = nullptr
	)
	{
		ID3DBlob* errors;
		HRESULT res = D3DCompileFromFile((SHADER_FOLDER + shaderName).c_str(),
			macros,
			includes,
			entrypoint,
			pTarget,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
			0,
			&byteCodes,
			&errors);

		if (FAILED(res)) {
			// If the shader failed to compile it should have written something to the error message.
			if (errors) {
				char* compileErrors = (char*)(errors->GetBufferPointer());

				std::cout << compileErrors << std::endl;
			}
			// If there was  nothing in the error message then it simply could not find the shader file itself.
			else
			{
				MessageBox(Game::Instance->Display->handle, (SHADER_FOLDER + shaderName).c_str(), L"Missing Shader File", MB_OK);
			}

			exit(1);
		}
	}

public:
	virtual void initialize(Microsoft::WRL::ComPtr<ID3D11Device> device) {}

	ShaderType* Get() const
	{
		return shader;
	}

	ID3DBlob* Bytecode() const
	{
		return byteCodes;
	}
	
	virtual ~Shader()
	{
		if (shader != nullptr)
		{
			shader->Release();
		}
	}
};

namespace ShaderTargets
{
	static const char PS_TARGET[] = "ps_5_0";
	static const char VS_TARGET[] = "vs_5_0";
}

class PixelShader
	: public Shader<ID3D11PixelShader, ShaderTargets::PS_TARGET>
{
public:
	PixelShader(const std::wstring& shaderName, const char* entrypoint = "PSMain",
		D3D_SHADER_MACRO* macros = nullptr, ID3DInclude* includes = nullptr)
		: Shader<struct ID3D11PixelShader, ShaderTargets::PS_TARGET>(shaderName, entrypoint, macros, includes)
	{
	}
	
	void initialize(Microsoft::WRL::ComPtr<ID3D11Device> device) override
	{
		device->CreatePixelShader(
			byteCodes->GetBufferPointer(),
			byteCodes->GetBufferSize(),
			nullptr, &shader);
	};
};

class VertexShader
	: public Shader<ID3D11VertexShader, ShaderTargets::VS_TARGET>
{
public:
	VertexShader(const std::wstring& shaderName, const char* entrypoint = "VSMain",
		D3D_SHADER_MACRO* macros = nullptr, ID3DInclude* includes = nullptr)
		: Shader<struct ID3D11VertexShader, ShaderTargets::VS_TARGET>(shaderName, entrypoint, macros, includes)
	{
	}

	void initialize(Microsoft::WRL::ComPtr<ID3D11Device> device) override
	{
		device->CreateVertexShader(
			byteCodes->GetBufferPointer(),
			byteCodes->GetBufferSize(),
			nullptr, &shader);
	};
};

