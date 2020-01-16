// Fill out your copyright notice in the Description page of Project Settings.
#include "DXManager.h"
#include "Util.h"

bool DXManager::Initialize(int width, int height)
{
	HRESULT result;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	D3D11_TEXTURE2D_DESC depthStencilTexDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	D3D11_BLEND_DESC blendStateDesc;
	D3D11_SAMPLER_DESC samplerStateDesc;
	D3D11_RASTERIZER_DESC rasterStateDesc;

	// 0.Create Device and DeviceContext
	result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &pDevice, NULL, &pDeviceContext);
	RETURN_FALSE_IF_ERROR(result, CreateDeviceAndContext);

	// 1.Create  DepthStencilTexture
	INIT_MEMORY(depthStencilTexDesc);
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTexDesc.CPUAccessFlags = 0;
	depthStencilTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTexDesc.Height = height;
	depthStencilTexDesc.Width = width;
	depthStencilTexDesc.MipLevels = 1;
	depthStencilTexDesc.MiscFlags = 0;
	depthStencilTexDesc.SampleDesc.Count = 1;
	depthStencilTexDesc.SampleDesc.Quality = 0;
	depthStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;

	result = pDevice->CreateTexture2D(&depthStencilTexDesc, nullptr, &pDepthStencilTexture);
	RETURN_FALSE_IF_ERROR(result, CreateDepthStencilTexture);

	// 2.Create DSV
	INIT_MEMORY(depthStencilViewDesc);
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Format = depthStencilTexDesc.Format;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	result = pDevice->CreateDepthStencilView(pDepthStencilTexture, &depthStencilViewDesc, &pDepthStencilView);
	RETURN_FALSE_IF_ERROR(result, CreateDepthStencilView);

	// 3.Create DepthEnable DSS
	INIT_MEMORY(depthStencilStateDesc);
	depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.StencilEnable = true;
	depthStencilStateDesc.StencilReadMask = 0xFF;
	depthStencilStateDesc.StencilWriteMask = 0xFF;

	result = pDevice->CreateDepthStencilState(&depthStencilStateDesc, &pDepthEnableStencilState);
	RETURN_FALSE_IF_ERROR(result, CreateDepthEnableStencilState);

	// 4.Create DepthDisable DSS
	depthStencilStateDesc.DepthEnable = false;

	result = pDevice->CreateDepthStencilState(&depthStencilStateDesc, &pDepthDisableStencilState);
	RETURN_FALSE_IF_ERROR(result, CreateDepthDisableStencilState);

	// 5.Create AlphaEnable BS
	INIT_MEMORY(blendStateDesc);
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = pDevice->CreateBlendState(&blendStateDesc, &pAlphaEnableBlendingState);
	RETURN_FALSE_IF_ERROR(result, CreateBlendEnableBlendState);

	// 6.Create AlphaDisable BS
	blendStateDesc.RenderTarget[0].BlendEnable = false;
	result = pDevice->CreateBlendState(&blendStateDesc, &pAlphaDisableBlendingState);
	RETURN_FALSE_IF_ERROR(result, CreateBlendDisableBlendState);

	// 7.Create SamplerState
	INIT_MEMORY(samplerStateDesc);
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.BorderColor[0] = 0.0f;
	samplerStateDesc.BorderColor[1] = 0.0f;
	samplerStateDesc.BorderColor[2] = 0.0f;
	samplerStateDesc.BorderColor[3] = 0.0f;
	samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.MaxAnisotropy = 1;
	samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerStateDesc.MinLOD = 0;
	samplerStateDesc.MipLODBias = 0.0f;

	result = pDevice->CreateSamplerState(&samplerStateDesc, &pSamplerState);
	RETURN_FALSE_IF_ERROR(result, CreateSamplerState);

	// 8.Create RasterizerState
	INIT_MEMORY(rasterStateDesc);
	rasterStateDesc.AntialiasedLineEnable = false;
	rasterStateDesc.CullMode = D3D11_CULL_BACK;
	rasterStateDesc.DepthBias = 0;
	rasterStateDesc.DepthBiasClamp = 0.0f;
	rasterStateDesc.DepthClipEnable = true;
	rasterStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterStateDesc.FrontCounterClockwise = false;
	rasterStateDesc.MultisampleEnable = false;
	rasterStateDesc.ScissorEnable = false;
	rasterStateDesc.SlopeScaledDepthBias = 0.0f;

	result = pDevice->CreateRasterizerState(&rasterStateDesc, &pRasterState);
	RETURN_FALSE_IF_ERROR(result, CreateRasterizerState);

	//9.Config
	pDeviceContext->RSSetState(pRasterState);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	pDeviceContext->RSSetViewports(1, &viewport);

	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	pDeviceContext->OMSetBlendState(pAlphaDisableBlendingState, blendFactor, 0xFFFFFFFF);
	pDeviceContext->OMSetDepthStencilState(pDepthEnableStencilState, 1);

	DXManagerInstance = this;
	bIntialized = true;
	LOG("DXManager Initialize Success");
	return true;
 }

 void DXManager::Destroy()
 {
	 SAFE_RELEASE(pDevice);
	 pDeviceContext->ClearState();
	 SAFE_RELEASE(pDeviceContext);
	 SAFE_RELEASE(pDepthStencilTexture);
	 SAFE_RELEASE(pDepthStencilView);
	 SAFE_RELEASE(pDepthEnableStencilState);
	 SAFE_RELEASE(pDepthDisableStencilState);
	 SAFE_RELEASE(pAlphaEnableBlendingState);
	 SAFE_RELEASE(pAlphaDisableBlendingState);
	 SAFE_RELEASE(pSamplerState);
	 SAFE_RELEASE(pRasterState);
 }

 void DXManager::InitializeScene(float r=0.2f, float g=0.2f, float b=0.2f, float a=1.0f)
 {
	 if (pCurrentRTV) {

		 float color[4];

		 // Setup the color to clear the buffer to.
		 color[2] = r;
		 color[0] = g;
		 color[1] = b;
		 color[3] = a;

		 // Clear the back buffer.
		 pDeviceContext->ClearRenderTargetView(pCurrentRTV, color);

		 // Clear the depth buffer.
		 pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	 }
 }

 void DXManager::SetRenderTargetView(ID3D11RenderTargetView * target)
 {
	 pDeviceContext->OMSetRenderTargets(1, &target, pDepthStencilView);
	 pCurrentRTV = target;
 }

 void DXManager::TurnOnZBuffer()
 {
	 pDeviceContext->OMSetDepthStencilState(pDepthEnableStencilState, 1);
 }

 void DXManager::TurnOffZBuffer()
 {
	 pDeviceContext->OMSetDepthStencilState(pDepthDisableStencilState, 1);

 }

 void DXManager::TurnOnAlphaBlend()
 {
	 float blendFactor[4];

	 // Setup the blend factor.
	 blendFactor[0] = 0.0f;
	 blendFactor[1] = 0.0f;
	 blendFactor[2] = 0.0f;
	 blendFactor[3] = 0.0f;

	 pDeviceContext->OMSetBlendState(pAlphaEnableBlendingState, blendFactor, 0xffffffff);
 }

 void DXManager::TurnOffAlphaBlend()
 {
	 float blendFactor[4];

	 // Setup the blend factor.
	 blendFactor[0] = 0.0f;
	 blendFactor[1] = 0.0f;
	 blendFactor[2] = 0.0f;
	 blendFactor[3] = 0.0f;

	 pDeviceContext->OMSetBlendState(pAlphaDisableBlendingState, blendFactor, 0xffffffff);
 }

 DXManager * GetDXManagerInstance()
 {
	 return DXManagerInstance;
 }
