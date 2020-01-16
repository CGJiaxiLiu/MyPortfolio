// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include <d3d11.h>
#include <d3dx10math.h>

class D3DManager;


class DXRENDER_API PolygonData
{
private:
	struct VertexDataType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tanget;
		D3DXVECTOR2 uv;
	};

public:
	void SetD3DManager(D3DManager* InManager) { m_D3DManager = InManager; }
	bool Initialize();
	void Update();
	void Destroy();
	void AddGeometryData(struct FStaticMeshLODResources * resource);
	int GetIndexCount() { return m_indexCount; }

private:
	D3DManager* m_D3DManager = 0;

	bool InitializeBuffer(ID3D11Device * device);
	void RenderBuffer(ID3D11DeviceContext * deviceContext);

	TArray<VertexDataType> m_vectexArr;
	TArray<unsigned long> m_indexArr;

	ID3D11Buffer *m_vertexBuffer = 0, *m_indexBuffer = 0;
	int m_vertexCount = 0, m_indexCount = 0;
	bool bInitilized = false;
};
