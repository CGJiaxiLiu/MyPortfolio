#pragma once

#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include "dxgi.h"
#include "d3dcommon.h"
#include "d3d11.h"


class DXRENDERPLUGIN_API DXManager
{
public:
	bool Initialize(int width, int height);
	void Destroy();
	void InitializeScene(float r, float g, float b, float a);
	ID3D11Device* GetDevice() { return pDevice; }
	ID3D11DeviceContext* GetContext() { return pDeviceContext; }
	ID3D11RenderTargetView* GetRenderTargetView() { return pCurrentRTV; }
	ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }
	void SetRenderTargetView(ID3D11RenderTargetView* target);
	void TurnOnZBuffer();
	void TurnOffZBuffer();
	void TurnOnAlphaBlend();
	void TurnOffAlphaBlend();


private:
	bool bIntialized = false;

	ID3D11Device* pDevice = 0;
	ID3D11DeviceContext* pDeviceContext = 0;

	ID3D11Texture2D* pDepthStencilTexture = 0;
	ID3D11DepthStencilView* pDepthStencilView = 0;
	ID3D11DepthStencilState* pDepthEnableStencilState = 0;
	ID3D11DepthStencilState* pDepthDisableStencilState = 0;
	ID3D11BlendState* pAlphaEnableBlendingState;
	ID3D11BlendState* pAlphaDisableBlendingState;
	ID3D11SamplerState* pSamplerState = 0;
	ID3D11RasterizerState* pRasterState = 0;

	ID3D11RenderTargetView* pCurrentRTV = 0;
};

static DXManager* DXManagerInstance = 0;
DXManager* GetDXManagerInstance();