#pragma once
#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

class DXRENDERPLUGIN_API ShaderManager
{
public:
	struct VSBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX inversedWorld;
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXVECTOR3 viewPosition;
		float padding;
	};

	struct PSBufferType
	{
		float intensity;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR3 padding;
	};

	bool Initialize(FString inVSFileName, FString inPSFileName);
	bool UpdateShaderParameters();
	void Render(uint32 indexCount, uint32 startIndex);
	void Destroy();

	VSBufferType* VSConstBuffer;
	PSBufferType* PSConstBuffer;

protected:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* shaderFilename);

	FString mVSFileName;
	FString mPSFileName;

	ID3D11VertexShader* pVertexShader = 0;
	ID3D11PixelShader* pPixelShader = 0;
	ID3D11InputLayout* pInputLayout = 0;
	ID3D11Buffer* pVSConstBuffer = 0;
	ID3D11Buffer*pPSConstBuffer = 0;

	bool bIntialized = false;
};
