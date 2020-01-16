// Fill out your copyright notice in the Description page of Project Settings.
#include "Displayer.h"
#include "DXManager.h"
#include "Util.h"
#include "Engine/TextureRenderTarget2D.h"

bool Displayer::Initialize(int width, int height, UTextureRenderTarget2D * inURenderTarget)
{
	if (!GetDXManagerInstance()) {
		return false;
	}

	auto DXManager = GetDXManagerInstance();
	
	HRESULT result;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;


	// 0.Create Staging Buffer
	INIT_MEMORY(textureDesc);
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = 0;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_STAGING;

	result = DXManager->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pStagingTexture);
	RETURN_FALSE_IF_ERROR(result, CreateStagingTexture);

	// 1.Create RenderTarget Texture
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	result = DXManager->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pRenderTargetTexture);
	RETURN_FALSE_IF_ERROR(result, CreateRenderTargetTexture);

	// 2.Create RenderTargetView
	INIT_MEMORY(renderTargetViewDesc);
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	result = DXManager->GetDevice()->CreateRenderTargetView(pRenderTargetTexture, &renderTargetViewDesc, &pRenderTargetView);
	RETURN_FALSE_IF_ERROR(result, CreateRenderTargetView);

	pUERenderTarget = inURenderTarget;
	if (!inURenderTarget) {
		return false;
	}
	aPixelData.Init(0, width * height * 4);
	bInitialized = true;
	LOG("Displayer Initialize Success");
	return true;
}

void Displayer::Destroy()
{
	SAFE_RELEASE(pStagingTexture);
	SAFE_RELEASE(pRenderTargetTexture);
	SAFE_RELEASE(pRenderTargetView);
}

void Displayer::Display()
{
	if (bInitialized) {
		if (CopyDXResource()) {
			RenderTexture();
		}
	}
}

bool Displayer::CopyDXResource()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	GetDXManagerInstance()->GetContext()->CopyResource(pStagingTexture, pRenderTargetTexture);
	HRESULT result = GetDXManagerInstance()->GetContext()->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);

	if (!FAILED(result)) {
		uint8* dataPtr = reinterpret_cast<uint8*>(mappedResource.pData);
		memcpy(aPixelData.GetData(), dataPtr, aPixelData.GetAllocatedSize());
		GetDXManagerInstance()->GetContext()->Unmap(pStagingTexture, 0);
		return true;
	}
	else {
		LOG("Map StagingTexture Fail");
		GetDXManagerInstance()->GetContext()->Unmap(pStagingTexture, 0);
		return false;
	}
}

void Displayer::RenderTexture()
{
	FUpdateTextureRegion2D* region = new FUpdateTextureRegion2D(0, 0, 0, 0, pUERenderTarget->SizeX, pUERenderTarget->SizeY);

	FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;
	RegionData->TextureResource = (FTextureRenderTarget2DResource*)pUERenderTarget->Resource;
	RegionData->Regions = region;
	RegionData->SrcPitch = pUERenderTarget->SizeX * 4 * aPixelData.GetTypeSize();
	RegionData->SrcBpp = 4 * aPixelData.GetTypeSize();
	RegionData->SrcData = (uint8*)aPixelData.GetData();

	if (pUERenderTarget && pUERenderTarget->Resource) {
		ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)(
			[RegionData](FRHICommandList& RHICmdList)
		{
			RHIUpdateTexture2D(
				RegionData->TextureResource->GetTextureRHI(),
				0,
				*RegionData->Regions,
				RegionData->SrcPitch,
				RegionData->SrcData
			);
			delete RegionData;
		});
	}
}
