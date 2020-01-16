// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

class D3DManager;
class PolygonData;

class DXRENDER_API ShaderManager
{
public:
	struct VSBufferType
	{
		D3DXMATRIX world;
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

	void SetD3DManager(D3DManager* InManager) { m_D3DManager = InManager; }
	void SetPolygonData(PolygonData* InData) { m_polygonData = InData; }
	void SetVSFileName(FString vsName) { m_VSFileName = vsName; }
	void SetPSFileName(FString psName) { m_PSFileName = psName; }
	bool Initialize();
	bool UpdateShaderParameters();
	void Render();
	void Destroy();

	VSBufferType* VSConstBuffer;
	PSBufferType* PSConstBuffer;
	class UTexture* m_textureInMaterial;

protected:
	bool InitializeShader(ID3D11Device* device, const WCHAR* vsFilename, const WCHAR* psFilename);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* shaderFilename);

	D3DManager* m_D3DManager = 0;
	PolygonData* m_polygonData = 0;
	FString m_VSFileName;
	FString m_PSFileName;

	ID3D11VertexShader* m_vertexShader = 0;
	ID3D11PixelShader* m_pixelShader = 0;
	ID3D11InputLayout* m_layout = 0;
	ID3D11Buffer* m_VSBuffer = 0;
	ID3D11Buffer* m_PSBuffer = 0;
	ID3D11SamplerState* m_sampleState = 0;

	ID3D11ShaderResourceView* m_textureShaderResourceView = 0;
	ID3D11Texture2D* m_textureResource = 0;
	TArray<uint8> dataArr;

	bool bIntialized = false;
};
