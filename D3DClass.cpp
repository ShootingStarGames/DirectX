#include "stdafx.h"
#include "D3DClass.h"


D3DClass::D3DClass()
{
	swapChain = 0;
	device = 0;
	deviceContext = 0;
	renderTargetView = 0;
	depthStencilBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;
}

D3DClass::D3DClass(const D3DClass &)
{
}


D3DClass::~D3DClass()
{
}


bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screendepth, float screenNear)
{
	HRESULT result;

	//�׷��� �������̽� ���丮
	IDXGIFactory* factory = nullptr;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}
	//�׷��� ī�� �������̽� ���
	IDXGIAdapter* adapter = nullptr;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}
	//���(�����)�� ���� ���
	IDXGIOutput* adapterOutput = nullptr;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}
	//���(�����)�� ���� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� ��
	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}
	//������ ��� ����Ϳ� �׷��� ī���� ���� ����Ʈ
	DXGI_MODE_DESC* displayModeList;
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}
	//����Ʈ ä���
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//���� ī�� ����ü
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
	{
		return false;
	}

	//���� ī�� �޸� �뷮 ������ �ް� ����Ʈ ������ ����
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	//���� ī�� �̸� ����
	size_t stringLength = 0;
	if (wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128) != 0)
	{
		return false;
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	//����ü�� ����ü �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//����۸� 1���� ���
	swapChainDesc.BufferCount = 1;

	//������� ���̿� ����
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//32bit �����̽� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//������� ���ΰ�ħ ������ ����
	if (vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//������� ���뵵
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	//�������� ���� ������ �ڵ鷯
	swapChainDesc.OutputWindow = hwnd;

	//��Ƽ ���ø� ����
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	//��ĵ ���� ���� �� ũ�⸦ �������� �������� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//��µ� ���� ����۸� ���쵵�� �����մϴ�.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//�߰� �ɼ� �÷��׸� ������� �ʽ��ϴ�.
	swapChainDesc.Flags = 0;

	//���� ������ directx11�� ����
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	//���� ü�� directx ��ġ �� ��ġ ���ؽ�Ʈ ����
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);

	if (FAILED(result))
	{
		return false;
	}

	//����� ������
	ID3D11Texture2D* backbufferptr = nullptr;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbufferptr);
	
	if (FAILED(result))
	{
		return false;
	}

	//����� �����ͷ� ���� Ÿ�� �並 ����
	result = device->CreateRenderTargetView(backbufferptr, NULL, &renderTargetView);


	if (FAILED(result))
	{
		return false;
	}

	//����� ������ ����
	backbufferptr->Release();
	backbufferptr = 0;
	
	//���� ���� ����ü �ʱ�ȭ
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//���� ���� ����ü �ۼ�
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//���� ���� ����ü�� �̿��Ͽ� ���� ���� �ؽ��� ����
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

	if (FAILED(result))
	{
		return false;
	}

	//���ٽ� ���� ����ü �ʱ�ȭ
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//���ٽ� ���� ����ü �ۼ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//�ȼ� ���� ���ٽ� ����
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ȼ� �ĸ� ���ٽ� ����
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	
	//���� ���ٽ� ���� ����ü ����
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//���� ���ٽ� �� ����ü �ۼ�
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� ���ٽ� �� ����
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//������ ��� ��� ���� ���ٽ� ���۸� ��� ������ ������ ���ο� ���ε�
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//�׷����� ������� ����� ������ ������ ����ü�� ����
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		return false;
	}

	//������ ������ ���¸� ����
	deviceContext->RSSetState(rasterState);

	//�������� ���� ����Ʈ
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//����Ʈ ����
	deviceContext->RSSetViewports(1, &viewport);

	//���� ��� ����
	float fieldOfView = 3.14159264f / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	//3D�������� ���� �������
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screendepth);
	//���� ����� ���� �׵����
	worldMatrix = XMMatrixIdentity();
	//2D�������� ���� ���� ���� ���
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screendepth);

	return true;
}


void D3DClass::Shutdown()
{
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}

	if (rasterState)
	{
		rasterState->Release();
		rasterState = 0;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = 0;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = 0;
	}

	if (device)
	{
		device->Release();
		device = 0;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = 0;
	}

}


void D3DClass::BegineScene(float red, float green, float blue, float alpha)
{
	float color[4] = { red,green,blue,alpha };

	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void D3DClass::EndScene()
{
	if (vsync_enabled)
	{
		swapChain->Present(1, 0);
	}
	else
	{
		swapChain->Present(0, 0);
	}
}

ID3D11Device * D3DClass::GetDevice()
{
	return device;
}

ID3D11DeviceContext * D3DClass::GetDeviceContext()
{
	return deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX &projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
}

void D3DClass::GetWorldMatrix(XMMATRIX &worldMatrix)
{
	worldMatrix = this->worldMatrix;
}

void D3DClass::GetOrthoMatrix(XMMATRIX &orthoMatrix)
{
	orthoMatrix = this->orthoMatrix;
}

void D3DClass::GetVideoCardInfo(char *cardName, int &memory)
{
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
}
