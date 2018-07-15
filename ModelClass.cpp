#include "stdafx.h"
#include "TextureClass.h"
#include "ModelClass.h"


ModelClass::ModelClass()
{
}

ModelClass::ModelClass(const ModelClass &)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device *device, LPCWSTR textureFilename)
{
	if (!InitializeBuffers(device))
	{
		return false;
	}
	return LoadTexture(device, textureFilename);
}

void ModelClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return indexCount;
}

ID3D11ShaderResourceView * ModelClass::GetTexture()
{
	return texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device *device)
{
	vertexCount = 3;
	indexCount = 3;

	Vertextype* vertices = new Vertextype[vertexCount];
	if (!vertices)
	{
		return false;
	}

	unsigned int* indices = new unsigned int[indexCount];
	if (!indices)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertextype) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(Vertextype) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
	{
		return false;
	}
	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}
	
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	UINT stride = sizeof(Vertextype);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device *device, LPCWSTR filename)
{
	texture = new TextureClass;
	if (!texture)
	{
		return false;
	}

	return texture->Initialize(device, filename);
}

void ModelClass::ReleaseTexture()
{
	if (texture)
	{
		texture->Shutdown();
		delete texture;
		texture = 0;
	}
}
