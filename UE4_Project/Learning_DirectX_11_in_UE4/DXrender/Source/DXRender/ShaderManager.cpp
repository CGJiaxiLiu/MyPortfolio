// Fill out your copyright notice in the Description page of Project Settings.


#include "ShaderManager.h"
#include "D3DManager.h"
#include "PolygonData.h"
#include "Dxerr.h"
#include "Engine/Texture2D.h"

bool ShaderManager::Initialize()
{
	if (!m_D3DManager || !*m_VSFileName || !*m_PSFileName || !m_polygonData) {
		return false;
	}

#undef UpdateResource

	if (m_textureInMaterial) {
		TextureCompressionSettings OldCompressionSettings = m_textureInMaterial->CompressionSettings;
		TextureMipGenSettings OldMipGenSettings = m_textureInMaterial->MipGenSettings;
		bool OldSRGB = m_textureInMaterial->SRGB;

		m_textureInMaterial->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		m_textureInMaterial->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		m_textureInMaterial->SRGB = false;
		m_textureInMaterial->UpdateResource();

		if (Cast<UTexture2D>(m_textureInMaterial)) {
			auto tex2D = Cast<UTexture2D>(m_textureInMaterial);
			UE_LOG(LogTemp, Warning, TEXT("Pixel Format: %d"), (int)tex2D->GetPixelFormat());
			if (auto dataPtr = tex2D->PlatformData->Mips[0].BulkData.LockReadOnly()) {
				int sizeX = tex2D->PlatformData->Mips[0].SizeX;
				int sizeY = tex2D->PlatformData->Mips[0].SizeY;
				dataArr.Init(0, sizeX * sizeY * 4);
				memcpy(dataArr.GetData(), dataPtr, sizeX * sizeY * 4);
				UE_LOG(LogTemp, Warning, TEXT("LockReadOnly Success"));
				tex2D->PlatformData->Mips[0].BulkData.Unlock();
			}
		}
		m_textureInMaterial->CompressionSettings = OldCompressionSettings;
		m_textureInMaterial->MipGenSettings = OldMipGenSettings;
		m_textureInMaterial->SRGB = OldSRGB;
		m_textureInMaterial->UpdateResource();
	}

	VSConstBuffer = new VSBufferType();
	PSConstBuffer = new PSBufferType();

	bIntialized = InitializeShader(m_D3DManager->GetDevice(), (TEXT("%s"), *m_VSFileName), (TEXT("%s"), *m_PSFileName));
	return bIntialized;
}

bool ShaderManager::UpdateShaderParameters()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VSBufferType* VSdataPtr;
	PSBufferType* PSdataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = m_D3DManager->GetDeviceContext()->Map(m_VSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("Map VSBuffer Fail: %s"), *describ);
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	VSdataPtr = (VSBufferType*)mappedResource.pData;
	memcpy(VSdataPtr, VSConstBuffer, sizeof(VSBufferType));

	// Unlock the constant buffer.
	m_D3DManager->GetDeviceContext()->Unmap(m_VSBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	m_D3DManager->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &m_VSBuffer);

	result = m_D3DManager->GetDeviceContext()->Map(m_PSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("Map PSBuffer Fail: %s"), *describ);
		return false;
	}

	PSdataPtr = (PSBufferType*)mappedResource.pData;
	memcpy(PSdataPtr, PSConstBuffer, sizeof(PSBufferType));

	m_D3DManager->GetDeviceContext()->Unmap(m_PSBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	m_D3DManager->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &m_PSBuffer);

	//UE_LOG(LogTemp, Warning, TEXT("Shader Update Parameter Success"));

	return true;
}

void ShaderManager::Render()
{
	RenderShader(m_D3DManager->GetDeviceContext(), m_polygonData->GetIndexCount());
}

void ShaderManager::Destroy()
{
	SAFE_RELEASE(m_sampleState);
	SAFE_RELEASE(m_VSBuffer);
	SAFE_RELEASE(m_PSBuffer);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_vertexShader);
	SAFE_RELEASE(m_textureShaderResourceView);
	SAFE_RELEASE(m_textureResource);
	delete VSConstBuffer;
	delete PSConstBuffer;
}

bool ShaderManager::InitializeShader(ID3D11Device* device, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC VSBufferDesc;
	D3D11_BUFFER_DESC PSBufferDesc;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing Shader File %s"), vsFilename);
		}
		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing Shader File %s"), vsFilename);
		}
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("CreateVertexShader Fail: %s"), *describ);
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("CreatePixelShader Fail: %s"), *describ);
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "BINORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "TEXCOORD";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);

	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("CreateInputLayout Fail: %s"), *describ);
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	VSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VSBufferDesc.ByteWidth = sizeof(VSBufferType);
	VSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	VSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VSBufferDesc.MiscFlags = 0;
	VSBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&VSBufferDesc, NULL, &m_VSBuffer);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("CreateVSBuffer Fail: %s"), *describ);
		return false;
	}

	PSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSBufferDesc.ByteWidth = sizeof(PSBufferType);
	PSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	PSBufferDesc.MiscFlags = 0;
	PSBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&PSBufferDesc, NULL, &m_PSBuffer);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("CreatePSBuffer Fail: %s"), *describ);
		return false;
	}



	auto tex2D = Cast<UTexture2D>(m_textureInMaterial);
	if (!tex2D) {
		return false;
	}
	int sizeX = tex2D->PlatformData->Mips[0].SizeX;
	int sizeY = tex2D->PlatformData->Mips[0].SizeY;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	// Setup the render target texture description.
	textureDesc.Width = sizeX;
	textureDesc.Height = sizeY;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA resourceData1;
	//resourceData1.pSysMem = static_cast<uint8_t*>(textureBlob1->GetBufferPointer());
	//resourceData1.SysMemPitch = texElementDesc.Width * sizeof(uint8_t) * 4;
	//resourceData1.SysMemSlicePitch = texElementDesc.Width * texElementDesc.Height * sizeof(uint8_t) * 4;

	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, &m_textureResource);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("Create Texture2D Fail: %s"), *describ);
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = m_D3DManager->GetDeviceContext()->Map(m_textureResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("Map Source Fail: %s"), *describ);
		return false;
	}

	uint8* dataPtr = reinterpret_cast<uint8*>(mappedResource.pData);
	for (int i = 0; i < sizeX*sizeY; i++) {
		dataPtr[i * 4] = dataArr[i * 4 + 2];
		dataPtr[i * 4 + 1] = dataArr[i * 4 + 1];
		dataPtr[i * 4 + 2] = dataArr[i * 4];
		dataPtr[i * 4 + 3] = dataArr[i * 4 + 3];
	}

	m_D3DManager->GetDeviceContext()->Unmap(m_textureResource, 0);

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_textureResource, &shaderResourceViewDesc, &m_textureShaderResourceView);
	if (FAILED(result))
	{
		FString describ = FString(DXGetErrorDescription(result));
		UE_LOG(LogTemp, Warning, TEXT("CreateShaderResourceView Fail: %s"), *describ);
		return false;
	}

	ID3D11ShaderResourceView* m_DEBUG_texture;
	result = D3DX11CreateShaderResourceViewFromFile(device, L"C:/Users/JiaxiLiu/Desktop/tex2.dds", NULL, NULL, &m_DEBUG_texture, NULL);

	ID3D11ShaderResourceView* m_base_texture;
	result = D3DX11CreateShaderResourceViewFromFile(device, L"C:/Users/JiaxiLiu/Desktop/base.dds", NULL, NULL, &m_base_texture, NULL);
	ID3D11ShaderResourceView* m_normal_texture;
	result = D3DX11CreateShaderResourceViewFromFile(device, L"C:/Users/JiaxiLiu/Desktop/normal.dds", NULL, NULL, &m_normal_texture, NULL);

	ID3D11ShaderResourceView* inputTextures[2];
	inputTextures[0] = m_base_texture;
	inputTextures[1] = m_normal_texture;
	m_D3DManager->GetDeviceContext()->PSSetShaderResources(0, 2, inputTextures);

	//m_D3DManager->GetDeviceContext()->PSSetShaderResources(0, 1, &m_DEBUG_texture);
	//m_D3DManager->GetDeviceContext()->PSSetShaderResources(0, 1, &m_textureShaderResourceView);


	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);

	if (FAILED(result))
	{
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Shader Intialize Success"));
	return true;
}

void ShaderManager::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

}

void ShaderManager::OutputShaderErrorMessage(ID3D10Blob * errorMessage, const WCHAR * shaderFilename)
{
	char* compileErrors;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());
	FString ErrorString = FString(compileErrors);

	UE_LOG(LogTemp, Warning, TEXT("%s Shader Error Message: %s"), shaderFilename, *ErrorString);
	errorMessage->Release();
	errorMessage = 0;
}
