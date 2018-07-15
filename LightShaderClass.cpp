#include "stdafx.h"
#include "LightShaderClass.h"


LightShaderClass::LightShaderClass()
{
}

LightShaderClass::LightShaderClass(const LightShaderClass &)
{
}


LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device *device, HWND hwnd)
{
	return InitializeShader(device, hwnd, L"./light.vs", L"./light.ps");
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
}

bool LightShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	if (!SetShdaerParameter(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColor))
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);
	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device *device, HWND hwnd, LPCWSTR vsFilename, LPCWSTR psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;

	//버텍스 셰이더 컴파일
	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "vs_main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else {
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//픽셀 셰이더 컴파일
	ID3D10Blob* pixelShaderBuffer = nullptr;
 	result = D3DCompileFromFile(psFilename, NULL, NULL, "ps_main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else {
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//컴파일 하였으므로 셰이더 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	//입력레이아웃 설정
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;
	
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//정점 입력 레이아웃
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	//셰이더와 입력 레이아웃을 만들었음으로 셰이더 버퍼는 필요없어짐
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//텍스쳐 샘플러 상태 구조체 생성 및 설정
	D3D11_SAMPLER_DESC samplerDesc;
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

	//텍스쳐 샘플러 상태 생성
	result = device->CreateSamplerState(&samplerDesc, &sampleState);
	if (FAILED(result))
	{
		return false;
	}
	
	//정점 셰이더가 가지고 있는 상수 구조체 생성 및 설정
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//정점 셰이더가 가지고 있는 광원 구조체 생성 및 설정

	D3D11_BUFFER_DESC lightBufferDesc;

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShaderClass::ShutdownShader()
{
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the layout.
	if (inputLayout)
	{
		inputLayout->Release();
		inputLayout = 0;
	}

	// Release the pixel shader.
	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = 0;
	}

	// Release the vertex shader.
	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = 0;
	}

	return;
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hwnd, LPCWSTR shaderFilename)
{
	OutputDebugString(reinterpret_cast<const wchar_t*>(errorMessage->GetBufferPointer()));

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader", shaderFilename, MB_OK);
}

bool LightShaderClass::SetShdaerParameter(ID3D11DeviceContext *deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	//상수에 접근할 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(matrixBuffer, 0);
	//정점 셰이더에서 상수 버퍼의 위치
	unsigned int bufferNumber = 0;
	//정점 셰이더에 상수 버퍼를 바뀐 값으로 적용
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	//픽셀 셰이더에 텍스쳐 리소스를 설정
	deviceContext->PSSetShaderResources(0, 1, &texture);

	if (FAILED(deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;
	
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	deviceContext->Unmap(lightBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);

	return false;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext *deviceContext, int indexCount)
{
	//입력 레이아웃 설정
	deviceContext->IAGetInputLayout(&inputLayout);
	//셰이더 설정
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &sampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}
