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
	// 텍스처를 파일로부터 읽어온다
	if (FAILED(CreateDDSTextureFromFile(device, filename, nullptr, &texture)))
	{
		return false;
	}

	return true;
}
void TextureClass::Shutdown()
{
	//텍스처 뷰 리소스를 해제한다.
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
