#include "GeometryDataManager.h"
#include "DXManager.h"
#include "Util.h"
#include "Runtime/Engine/Public/StaticMeshResources.h"
#include "Engine/Texture2D.h"

bool GeometryDataManager::Intialize()
{
	int width = mTextureWidth;
	int height = mTextureHeight;

	auto DXManagerInstance = GetDXManagerInstance();
	if (!DXManagerInstance) {
		return false;
	}
	if (aVertexData.Num() == 0 || aIndexData.Num() == 0) {
		LOG("Geometry Data is NULL");
		return false;
	}

	HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_SUBRESOURCE_DATA * textureData;
	textureData = new D3D11_SUBRESOURCE_DATA[aMetaData.Num()];

	int mVertexCount = aVertexData.Num();
	int mIndexCount = aIndexData.Num();

	// 0.Create and Intialize Vertex Buffer
	INIT_MEMORY(vertexBufferDesc);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexAttribute) * mVertexCount;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	vertexData.pSysMem = aVertexData.GetData();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = GetDXManagerInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	RETURN_FALSE_IF_ERROR(result, CreateVertexBuffer);

	// 1.Create and Intialize Index Buffer
	INIT_MEMORY(indexBufferDesc);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(uint32) * mIndexCount;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	indexData.pSysMem = aIndexData.GetData();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = GetDXManagerInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	RETURN_FALSE_IF_ERROR(result, CreateIndexBuffer);

	// 2.Create an Intialize TextureResource
	INIT_MEMORY(textureDesc);
	textureDesc.ArraySize = aMetaData.Num();
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	LOG("Meta Count: %d", aMetaData.Num());

	//INIT_MEMORY(textureData);
	for (int i = 0; i < aMetaData.Num(); i++) {
		textureData[i].pSysMem = aTexturePixelData.GetData() + width * height * 4 * i;
		textureData[i].SysMemPitch = width * aTexturePixelData.GetTypeSize() * 4;
		textureData[i].SysMemSlicePitch = textureData[i].SysMemPitch * height;
	}

	result = GetDXManagerInstance()->GetDevice()->CreateTexture2D(&textureDesc, textureData, &pTextureArray);
	RETURN_FALSE_IF_ERROR(result, CreateTexture2DArray);

	INIT_MEMORY(shaderResourceViewDesc);
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Texture2DArray.ArraySize = textureDesc.ArraySize;
	shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1;
	shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;

	result = GetDXManagerInstance()->GetDevice()->CreateShaderResourceView(pTextureArray, &shaderResourceViewDesc, &pTextureShaderResourceView);
	RETURN_FALSE_IF_ERROR(result, CreateShaderResourceView);

	// 3.Config
	uint32 stride = sizeof(VertexAttribute);
	uint32 offset = 0;

	GetDXManagerInstance()->GetContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	GetDXManagerInstance()->GetContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	GetDXManagerInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetDXManagerInstance()->GetContext()->PSSetShaderResources(0, 1, &pTextureShaderResourceView);

	LOG("GeometryDataManager Initialize Success");
	delete[] textureData;
	textureData = 0;
	return true;
}

void GeometryDataManager::Destroy()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pTextureShaderResourceView);
	SAFE_RELEASE(pTextureArray);
}

void GeometryDataManager::AddGeometryData(FStaticMeshLODResources * resource, class UTexture2D* inTexture)
{
#undef UpdateResource

	if (inTexture) {
		TextureCompressionSettings OldCompressionSettings = inTexture->CompressionSettings;
		TextureMipGenSettings OldMipGenSettings = inTexture->MipGenSettings;
		bool OldSRGB = inTexture->SRGB;

		inTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		inTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		inTexture->SRGB = false;
		inTexture->UpdateResource();

		if (Cast<UTexture2D>(inTexture)) {
			auto tex2D = Cast<UTexture2D>(inTexture);
			if (auto dataPtr = tex2D->PlatformData->Mips[0].BulkData.LockReadOnly()) {
				int sizeX = tex2D->PlatformData->Mips[0].SizeX;
				int sizeY = tex2D->PlatformData->Mips[0].SizeY;

				if (mTextureWidth == 0 && mTextureHeight == 0) {
					mTextureWidth = sizeX;
					mTextureHeight = sizeY;
				}
				else {
					if (sizeX != mTextureWidth || sizeY != mTextureHeight) {
						tex2D->PlatformData->Mips[0].BulkData.Unlock();
						inTexture->CompressionSettings = OldCompressionSettings;
						inTexture->MipGenSettings = OldMipGenSettings;
						inTexture->SRGB = OldSRGB;
						inTexture->UpdateResource();
						return;
					}
				}

				TArray<uint8> aTempPixelData;
				aTempPixelData.Init(0, sizeX * sizeY * 4);
				memcpy(aTempPixelData.GetData(), dataPtr, sizeX * sizeY * 4);
				aTexturePixelData.Append(aTempPixelData);
				tex2D->PlatformData->Mips[0].BulkData.Unlock();
				LOG("Add Texture");
			}
		}
		inTexture->CompressionSettings = OldCompressionSettings;
		inTexture->MipGenSettings = OldMipGenSettings;
		inTexture->SRGB = OldSRGB;
		inTexture->UpdateResource();
	}

	uint32 indexCount = resource->IndexBuffer.GetNumIndices();
	uint32 vertexCount = resource->VertexBuffers.StaticMeshVertexBuffer.GetNumVertices();

	TArray<VertexAttribute> aTempVertex;
	TArray<uint32> aTempIndex;

	aTempVertex.Init(VertexAttribute(), vertexCount);
	aTempIndex.Init(0, indexCount);

	auto vertexData = &resource->VertexBuffers;
	for (uint32 i = 0; i < vertexCount; i++) {
		VertexAttribute* temp = &aTempVertex[i];
		temp->position.x = vertexData->PositionVertexBuffer.VertexPosition(i).Y / 100.0f;
		temp->position.y = vertexData->PositionVertexBuffer.VertexPosition(i).Z / 100.0f;
		temp->position.z = vertexData->PositionVertexBuffer.VertexPosition(i).X / 100.0f;
		temp->normal.x = vertexData->StaticMeshVertexBuffer.VertexTangentZ(i).Y;
		temp->normal.y = vertexData->StaticMeshVertexBuffer.VertexTangentZ(i).Z;
		temp->normal.z = vertexData->StaticMeshVertexBuffer.VertexTangentZ(i).X;
		temp->binormal.x = vertexData->StaticMeshVertexBuffer.VertexTangentY(i).Y;
		temp->binormal.y = vertexData->StaticMeshVertexBuffer.VertexTangentY(i).Z;
		temp->binormal.z = vertexData->StaticMeshVertexBuffer.VertexTangentY(i).X;
		temp->tanget.x = vertexData->StaticMeshVertexBuffer.VertexTangentX(i).Y;
		temp->tanget.y = vertexData->StaticMeshVertexBuffer.VertexTangentX(i).Z;
		temp->tanget.z = vertexData->StaticMeshVertexBuffer.VertexTangentX(i).X;
		temp->uv.x = vertexData->StaticMeshVertexBuffer.GetVertexUV(i, 0).X;
		temp->uv.y = vertexData->StaticMeshVertexBuffer.GetVertexUV(i, 0).Y;
	}

	uint32 lastIndex = aVertexData.Num();
	for (uint32 i = 0; i < indexCount; i++) {
		if (i % 3 == 0) {
			aTempIndex[i] = resource->IndexBuffer.GetIndex(i) + lastIndex;
		}
		else if (i % 3 == 1) {
			aTempIndex[i] = resource->IndexBuffer.GetIndex(i + 1) + lastIndex;
		}
		else {
			aTempIndex[i] = resource->IndexBuffer.GetIndex(i - 1) + lastIndex;
		}
	}

	GeometryMetaData meta = GeometryMetaData();
	meta.length = indexCount;
	meta.startIndex = aIndexData.Num();
	aMetaData.Add(meta);

	aVertexData.Append(aTempVertex);
	aIndexData.Append(aTempIndex);
}

const GeometryMetaData* GeometryDataManager::GetMetaData(int geometryIndex)
{
	if (geometryIndex < aMetaData.Num()) {
		return &aMetaData[geometryIndex];
	}
	else {
		return nullptr;
	}
}

