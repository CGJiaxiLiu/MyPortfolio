// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualDisplayer.h"
#include "D3DManager.h"
#include "Engine/TextureRenderTarget2D.h"

#ifdef RENDER_IN_WINDOW
VisualDisplayer* VisualDisplayer::DisplayerPtr = 0;
#endif

bool VisualDisplayer::Initialize()
{
	if (!m_D3DManager) {
		return false;
	}
	if (!m_target) {
		return false;
	}

	ID3D11Device* device = m_D3DManager->GetDevice();

	m_Date.Init(0, m_target->SizeX * m_target->SizeY * 4);

	HRESULT result;
	D3D11_TEXTURE2D_DESC StagingTextureDesc;
	ZeroMemory(&StagingTextureDesc, sizeof(StagingTextureDesc));
	StagingTextureDesc.Width = m_target->SizeX;
	StagingTextureDesc.Height = m_target->SizeY;
	StagingTextureDesc.MipLevels = 1;
	StagingTextureDesc.ArraySize = 1;
	StagingTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	StagingTextureDesc.SampleDesc.Count = 1;
	StagingTextureDesc.SampleDesc.Quality = 0;
	StagingTextureDesc.Usage = D3D11_USAGE_STAGING;
	StagingTextureDesc.BindFlags = 0;
	StagingTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	StagingTextureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&StagingTextureDesc, NULL, &m_StagingBuffer);
	if (FAILED(result))
	{
		return false;
	}
	else {
		bInitializeSuccess = true;
	}


#ifdef RENDER_IN_WINDOW

	InitializeWindow(m_target->SizeX, m_target->SizeY);
	bInitializeSuccess = InitializeSwapChain(m_target->SizeX, m_target->SizeY);
	DisplayerPtr = this;

#endif // RENDER_IN_WINDOW

	if (bInitializeSuccess) {
		return true;
	}
	else {
		return false;
	}
}

void VisualDisplayer::DisplayOnTexture()
{
	if (!bInitializeSuccess) {
		return;
	}

	bool result = CopyResource(m_D3DManager->GetDeviceContext(), m_D3DManager->GetTargetTexture());
	if (result) {
		RenderTexture();
	}
}

void VisualDisplayer::Destroy()
{
	SAFE_RELEASE(m_StagingBuffer);

#ifdef RENDER_IN_WINDOW

	this->ShutdownWindow();

#endif // RENDER_IN_WINDOW
}


#ifdef RENDER_IN_WINDOW

void VisualDisplayer::ExecuteWindow(double DeltaTime)
{
	// Initialize the message structure.
	ZeroMemory(&m_msg, sizeof(MSG));

	// Handle the windows messages.
	if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}

	// If windows signals to end the application then exit out.
	if (m_msg.message == WM_QUIT)
	{
		this->ShutdownWindow();
	}
}

void VisualDisplayer::DisplayOnWindow()
{
	m_swapChain->Present(0, 0);
}

LRESULT VisualDisplayer::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (umsg == WM_DESTROY || umsg == WM_CLOSE) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void VisualDisplayer::SetRenderToWindow()
{
	m_D3DManager->GetDeviceContext()->OMSetRenderTargets(1, &m_windowRenderTargetView, m_D3DManager->GetDepthStencilView());
}

#endif // RENDER_IN_WINDOW


bool VisualDisplayer::CopyResource(ID3D11DeviceContext * context, ID3D11Texture2D * renderTargetTexture)
{
	
#ifdef RENDER_IN_WINDOW
	
	
#endif // RENDER_IN_WINDOW

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;

	// Get the pointer to the back buffer.
	context->CopyResource(m_StagingBuffer, renderTargetTexture);
	result = context->Map(m_StagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);

	if (!FAILED(result)) {
		float* dataPtr = reinterpret_cast<float*>(mappedResource.pData);
		memcpy(m_Date.GetData(), dataPtr, m_Date.GetAllocatedSize());
		context->Unmap(m_StagingBuffer, 0);
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Map Fail"));
		context->Unmap(m_StagingBuffer, 0);
		return false;
	}
}

void VisualDisplayer::RenderTexture()
{
	FUpdateTextureRegion2D* region = new FUpdateTextureRegion2D(0, 0, 0, 0, m_target->SizeX, m_target->SizeY);

	FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;
	RegionData->TextureResource = (FTextureRenderTarget2DResource*)m_target->Resource;
	RegionData->Regions = region;
	RegionData->SrcPitch = m_target->SizeX * 4 * m_Date.GetTypeSize();
	RegionData->SrcBpp = 4 * m_Date.GetTypeSize();
	RegionData->SrcData = (uint8*)m_Date.GetData();

	if (m_target && m_target->Resource) {
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

#ifdef RENDER_IN_WINDOW

void VisualDisplayer::InitializeWindow(unsigned int inWidth, unsigned int inHeight)
{
	WNDCLASSEX wc;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Render";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = this->MessageHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - inWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - inHeight) / 2;

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, inWidth, inHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
}

bool VisualDisplayer::InitializeSwapChain(unsigned int inWidth, unsigned int inHeight)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = inWidth;
	swapChainDesc.BufferDesc.Height = inHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	// Set the refresh rate of the back buffer.
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = this->GetWindowHandle();

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	IDXGIDevice * pDXGIDevice = nullptr;
	m_D3DManager->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter = nullptr;
	pDXGIDevice->GetAdapter(&pDXGIAdapter);

	IDXGIFactory * pIDXGIFactory = nullptr;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);

	result = pIDXGIFactory->CreateSwapChain(m_D3DManager->GetDevice(), &swapChainDesc, &m_swapChain);
	if (FAILED(result)) {
		return false;
	}

	// Get the pointer to the back buffer.
	result = this->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_D3DManager->GetDevice()->CreateRenderTargetView(backBufferPtr, NULL, &m_windowRenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	return true;
}

void VisualDisplayer::ShutdownWindow()
{
	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
}

#endif