// Fill out your copyright notice in the Description page of Project Settings.
#include "PolygonData.h"
#include "D3DManager.h"
#include "Runtime/Engine/Public/StaticMeshResources.h"

bool PolygonData::Initialize()
{
	if (!m_D3DManager) {
		return false;
	}
	bInitilized = InitializeBuffer(m_D3DManager->GetDevice());
	return bInitilized;
}

void PolygonData::Update()
{
	if (bInitilized) {
		RenderBuffer(m_D3DManager->GetDeviceContext());
	}
}

void PolygonData::Destroy()
{
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_vertexBuffer);
}

void PolygonData::AddGeometryData(struct FStaticMeshLODResources * resource)
{
	uint32 indexCount = resource->IndexBuffer.GetNumIndices();
	uint32 vertexCount = resource->VertexBuffers.StaticMeshVertexBuffer.GetNumVertices();

	m_indexArr.Init(0, indexCount);
	m_vectexArr.Init(VertexDataType(), vertexCount);

	auto vertexData = &resource->VertexBuffers;
	for (uint32 i = 0; i < vertexCount; i++) {
		VertexDataType temp = VertexDataType();
		temp.position.x = vertexData->PositionVertexBuffer.VertexPosition(i).Y / 100.0f;
		temp.position.y = vertexData->PositionVertexBuffer.VertexPosition(i).Z / 100.0f;
		temp.position.z = vertexData->PositionVertexBuffer.VertexPosition(i).X / 100.0f;
		temp.normal.x = vertexData->StaticMeshVertexBuffer.VertexTangentZ(i).Y;
		temp.normal.y = vertexData->StaticMeshVertexBuffer.VertexTangentZ(i).Z;
		temp.normal.z = vertexData->StaticMeshVertexBuffer.VertexTangentZ(i).X;
		temp.binormal.x = vertexData->StaticMeshVertexBuffer.VertexTangentY(i).Y;
		temp.binormal.y = vertexData->StaticMeshVertexBuffer.VertexTangentY(i).Z;
		temp.binormal.z = vertexData->StaticMeshVertexBuffer.VertexTangentY(i).X;
		temp.tanget.x = vertexData->StaticMeshVertexBuffer.VertexTangentX(i).Y;
		temp.tanget.y = vertexData->StaticMeshVertexBuffer.VertexTangentX(i).Z;
		temp.tanget.z = vertexData->StaticMeshVertexBuffer.VertexTangentX(i).X;
		temp.uv.x = vertexData->StaticMeshVertexBuffer.GetVertexUV(i, 0).X;
		temp.uv.y = vertexData->StaticMeshVertexBuffer.GetVertexUV(i, 0).Y;
		//UE_LOG(LogTemp, Warning, TEXT("UV: %f, %f"), temp.uv.x, temp.uv.y);
		m_vectexArr[i] = temp;
	}

	for (uint32 i = 0; i < indexCount; i++) {
		if (i % 3 == 0) {
			m_indexArr[i] = resource->IndexBuffer.GetIndex(i);
		}
		else if (i % 3 == 1) {
			m_indexArr[i] = resource->IndexBuffer.GetIndex(i + 1);
		}
		else {
			m_indexArr[i] = resource->IndexBuffer.GetIndex(i - 1);
		}
	}
}

bool PolygonData::InitializeBuffer(ID3D11Device * device)
{

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = m_vectexArr.Num();

	// Set the number of indices in the index array.
	m_indexCount = m_indexArr.Num();

	 //Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexDataType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_vectexArr.GetData();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = m_indexArr.GetData();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void PolygonData::RenderBuffer(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexDataType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
