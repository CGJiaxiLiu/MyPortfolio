#pragma once
#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include "dxgi.h"
#include "d3dcommon.h"
#include "d3d11.h"

struct FUpdateTextureRegionsData
{
	class FTextureRenderTarget2DResource* TextureResource;
	struct FUpdateTextureRegion2D* Regions;
	uint32 SrcPitch;
	uint32 SrcBpp;
	uint8* SrcData;
};

class UTextureRenderTarget2D;

class DXRENDERPLUGIN_API Displayer
{

public:
	bool Initialize(int width, int height, UTextureRenderTarget2D* inURenderTarget);
	void Destroy();
	void Display();
	ID3D11RenderTargetView * GetRenderTargetView() { return this->pRenderTargetView; }

private:
	bool CopyDXResource();
	void RenderTexture();

	bool bInitialized = false;
	UTextureRenderTarget2D* pUERenderTarget = 0;
	TArray<uint8> aPixelData;

	ID3D11Texture2D* pStagingTexture = 0;
	ID3D11Texture2D* pRenderTargetTexture = 0;
	ID3D11RenderTargetView* pRenderTargetView = 0;

};
