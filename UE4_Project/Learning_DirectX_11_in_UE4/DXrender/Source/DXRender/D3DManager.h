// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include "dxgi.h"
#include "d3dcommon.h"
#include "d3d11.h"


class DXRENDER_API D3DManager
{
public:
	bool Initialize(int width, int height);
	void Destroy();
	void DXBeginScene(float time);
	ID3D11Device* GetDevice() { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }
	ID3D11Texture2D* GetTargetTexture() { return m_renderTargetTexture;	}
	ID3D11DepthStencilView* GetDepthStencilView() { return m_depthStencilView; }
	void SetRenderToTexture();
	void TurnOnZBuffer();
	void TurnOffZBuffer();
	void TurnOnAlphaBlend();
	void TurnOffAlphaBlend();

protected:
	bool D3DIntializedSuccess = false;
	ID3D11Device* m_device = 0;
	ID3D11DeviceContext* m_deviceContext = 0;

	ID3D11Texture2D* m_renderTargetTexture = 0;
	ID3D11RenderTargetView* m_renderTargetView = 0;

	ID3D11Texture2D* m_depthStencilBuffer = 0;
	ID3D11DepthStencilState* m_depthStencilState = 0;
	ID3D11DepthStencilView* m_depthStencilView = 0;
	ID3D11RasterizerState* m_rasterState = 0;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;

};