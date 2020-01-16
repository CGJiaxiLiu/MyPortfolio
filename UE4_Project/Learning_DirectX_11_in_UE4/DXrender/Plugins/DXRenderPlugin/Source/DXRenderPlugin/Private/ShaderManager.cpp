#include "ShaderManager.h"
#include "Util.h"
#include "DXManager.h"
#include "GeometryDataManager.h"

bool ShaderManager::Initialize(FString inVSFileName, FString inPSFileName)
{
	VSConstBuffer = new VSBufferType();
	PSConstBuffer = new PSBufferType();

	auto VSFileName = (TEXT("%s"), *inVSFileName);
	auto PSFileName = (TEXT("%s"), *inPSFileName);

	HRESULT result;
	ID3D10Blob* errorMessage = 0;
	ID3D10Blob* vertexShaderBuffer = 0;
	ID3D10Blob* pixelShaderBuffer = 0;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC VSBufferDesc;
	D3D11_BUFFER_DESC PSBufferDesc;

	// 0.Compile the vertex and pixel shader code.
	result = D3DX11CompileFromFile(VSFileName, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, VSFileName);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			LOG("Missing Shader File %s", VSFileName);
		}
		return false;
	}

	result = D3DX11CompileFromFile(PSFileName, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, PSFileName);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing Shader File %s"), PSFileName);
		}
		return false;
	}

	
	// 1.Create the vertex and pixel shader from the buffer.
	result = GetDXManagerInstance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &pVertexShader);
	RETURN_FALSE_IF_ERROR(result, CreateVertexShader);

	result = GetDXManagerInstance()->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pPixelShader);
	RETURN_FALSE_IF_ERROR(result, CreatePixelShader);

	// 2.Create polygonLayout
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

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = GetDXManagerInstance()->GetDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &pInputLayout);
	RETURN_FALSE_IF_ERROR(result, CreateInputLayout);

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// 3.Create VS and PS const buffer
	VSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VSBufferDesc.ByteWidth = sizeof(VSBufferType);
	VSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	VSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VSBufferDesc.MiscFlags = 0;
	VSBufferDesc.StructureByteStride = 0;

	result = GetDXManagerInstance()->GetDevice()->CreateBuffer(&VSBufferDesc, NULL, &pVSConstBuffer);
	RETURN_FALSE_IF_ERROR(result, CreateVSBuffer);

	PSBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PSBufferDesc.ByteWidth = sizeof(PSBufferType);
	PSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	PSBufferDesc.MiscFlags = 0;
	PSBufferDesc.StructureByteStride = 0;

	result = GetDXManagerInstance()->GetDevice()->CreateBuffer(&PSBufferDesc, NULL, &pPSConstBuffer);
	RETURN_FALSE_IF_ERROR(result, CreatePSBuffer);

	// 4.Config
	GetDXManagerInstance()->GetContext()->IASetInputLayout(pInputLayout);
	GetDXManagerInstance()->GetContext()->VSSetShader(pVertexShader, NULL, 0);
	GetDXManagerInstance()->GetContext()->PSSetShader(pPixelShader, NULL, 0);

	bIntialized = true;
	LOG("RenderManager Initialize Success");
	return true;
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

void ShaderManager::Destroy()
{
	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pVSConstBuffer);
	SAFE_RELEASE(pPSConstBuffer);
	delete VSConstBuffer;
	VSConstBuffer = 0;
	delete PSConstBuffer;
	PSConstBuffer = 0;
}

bool ShaderManager::UpdateShaderParameters()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VSBufferType* VSdataPtr;
	PSBufferType* PSdataPtr;
	unsigned int bufferNumber;


	result = GetDXManagerInstance()->GetContext()->Map(pVSConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	RETURN_FALSE_IF_ERROR(result, MapVSBufferFail);

	VSdataPtr = (VSBufferType*)mappedResource.pData;
	memcpy(VSdataPtr, VSConstBuffer, sizeof(VSBufferType));

	GetDXManagerInstance()->GetContext()->Unmap(pVSConstBuffer, 0);

	bufferNumber = 0;

	GetDXManagerInstance()->GetContext()->VSSetConstantBuffers(bufferNumber, 1, &pVSConstBuffer);


	result = GetDXManagerInstance()->GetContext()->Map(pPSConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	RETURN_FALSE_IF_ERROR(result, MapPSBufferFail);

	PSdataPtr = (PSBufferType*)mappedResource.pData;
	memcpy(PSdataPtr, PSConstBuffer, sizeof(PSBufferType));

	GetDXManagerInstance()->GetContext()->Unmap(pPSConstBuffer, 0);

	bufferNumber = 0;

	GetDXManagerInstance()->GetContext()->PSSetConstantBuffers(bufferNumber, 1, &pPSConstBuffer);
	return true;
}

void ShaderManager::Render(uint32 indexCount, uint32 startIndex)
{
	GetDXManagerInstance()->GetContext()->DrawIndexed(indexCount, startIndex, 0);
}