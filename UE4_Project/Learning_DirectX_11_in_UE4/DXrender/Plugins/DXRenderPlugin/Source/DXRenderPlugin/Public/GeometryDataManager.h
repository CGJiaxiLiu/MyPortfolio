#pragma once
#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include <d3d11.h>
#include <d3dx10math.h>

struct GeometryMetaData
{
	uint32 startIndex;
	uint32 length;
};

class DXRENDERPLUGIN_API GeometryDataManager
{
private:
	struct VertexAttribute
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tanget;
		D3DXVECTOR2 uv;
	};

public:
	bool Intialize();
	void Destroy();
	void AddGeometryData(struct FStaticMeshLODResources * resource, class UTexture2D* inTexture);
	const GeometryMetaData* GetMetaData(int geometryIndex);
	int GetObjectCount() { return aMetaData.Num(); }

private:
	int mTextureWidth = 0;
	int mTextureHeight = 0;

	TArray<VertexAttribute> aVertexData;
	TArray<uint32> aIndexData;
	TArray<GeometryMetaData> aMetaData;
	TArray<uint8> aTexturePixelData;

	ID3D11Buffer * pVertexBuffer = 0;
	ID3D11Buffer * pIndexBuffer = 0;
	ID3D11ShaderResourceView* pTextureShaderResourceView = 0;
	ID3D11Texture2D* pTextureArray = 0;
};
