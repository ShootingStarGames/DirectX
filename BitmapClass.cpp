#include "stdafx.h"
#include "TextureClass.h"
#include "BitmapClass.h"


BitmapClass::BitmapClass()
{
}

BitmapClass::BitmapClass(const BitmapClass &)
{
}


BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device * device, int screenWidth, int screenHeight, WCHAR *textureFilename, int bitmapWidth, int bitmapHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

	if (!InitializeBuffers(device))
	{
		return false;	
	}
	return LoadTexture(device, textureFilename);
}

void BitmapClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();
}

bool BitmapClass::Render(ID3D11DeviceContext *deviceContext, int positionX, int positionY)
{
	if (!UpdateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}

	RenderBuffers(deviceContext);
	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device *device)
{
	m_indexCount = m_vertexCount = 6;

	VertexType*	vertices = new VertexType[m_vertexCount];
	
	if (!vertices)
	{
		return false;
	}
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	delete[] vertices;
	delete[] indices;
	vertices = 0;
	indices = 0;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext *deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;

	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	m_previousPosX = positionX;
	m_previousPosY = positionY;

	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;

	top = (float)(m_screenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// 두 번째 삼각형
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// 버텍스 버퍼를 쓸 수 있도록 잠급니다.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	verticesPtr = (VertexType*)mappedResource.pData;

	// 데이터를 정점 버퍼에 복사합니다.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// 정점 버퍼의 잠금을 해제합니다.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// 더 이상 필요하지 않은 꼭지점 배열을 해제합니다.
	delete[] vertices;
	vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool BitmapClass::LoadTexture(ID3D11Device *device, WCHAR *filename)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}
	return m_Texture->Initialize(device, filename);
}

void BitmapClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}
}
