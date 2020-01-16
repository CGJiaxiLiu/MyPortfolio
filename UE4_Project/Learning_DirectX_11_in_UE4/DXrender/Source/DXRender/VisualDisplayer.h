// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include "dxgi.h"
#include "d3dcommon.h"
#include "d3d11.h"

//#ifndef RENDER_IN_WINDOW
//#define RENDER_IN_WINDOW
//#endif

struct FUpdateTextureRegionsData
{
	FTextureRenderTarget2DResource* TextureResource;
	FUpdateTextureRegion2D* Regions;
	uint32 SrcPitch;
	uint32 SrcBpp;
	uint8* SrcData;
};

class UTextureRenderTarget2D;
class D3DManager;

class DXRENDER_API VisualDisplayer
{

public:
	void SetOutputTarget(UTextureRenderTarget2D* InTarget) { m_target = InTarget; }
	void SetD3DManager(D3DManager* InManager) { m_D3DManager = InManager; }
	bool Initialize();
	void DisplayOnTexture();
	void Destroy();

private:
	bool CopyResource(struct ID3D11DeviceContext* context, struct ID3D11Texture2D* renderTargetTexture);
	void RenderTexture();

	bool bInitializeSuccess = false;
	UTextureRenderTarget2D* m_target;
	D3DManager* m_D3DManager = 0;
	TArray<uint8> m_Date;
	struct ID3D11Texture2D* m_StagingBuffer = 0;


#ifdef RENDER_IN_WINDOW

public:
	HWND GetWindowHandle() { return m_hwnd; }
	void ExecuteWindow(double DeltaTime);
	void DisplayOnWindow();
	static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	static VisualDisplayer* DisplayerPtr;
	void SetRenderToWindow();
	ID3D11RenderTargetView* GetWindowRenderTargetView() { return m_windowRenderTargetView; }
protected:
	void InitializeWindow(unsigned int inWidth, unsigned int inHeight);
	bool InitializeSwapChain(unsigned int inWidth, unsigned int inHeight);
	void ShutdownWindow();

	LPCWSTR m_applicationName = NULL;
	HINSTANCE m_hinstance = NULL;
	HWND m_hwnd = NULL;
	MSG m_msg;

	IDXGISwapChain* m_swapChain = 0;
	ID3D11RenderTargetView* m_windowRenderTargetView = 0;

#endif // RENDER_IN_WINDOW

};
