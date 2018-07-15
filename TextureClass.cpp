#include "stdafx.h"
#include "TextureClass.h"


TextureClass::TextureClass()
{
}

TextureClass::TextureClass(const TextureClass &)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, LPCWSTR filename)
{
	// �ؽ�ó�� ���Ϸκ��� �о�´�
	if (FAILED(CreateDDSTextureFromFile(device, filename, nullptr, &texture)))
	{
		return false;
	}

	return true;
}
void TextureClass::Shutdown()
{
	//�ؽ�ó �� ���ҽ��� �����Ѵ�.
	if (texture)
	{
		texture->Release();
		texture = 0;
	}
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return texture;
}
