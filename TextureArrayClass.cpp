#include "stdafx.h"
#include "TextureArrayClass.h"


TextureArrayClass::TextureArrayClass()
{
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass &)
{
}


TextureArrayClass::~TextureArrayClass()
{
}

bool TextureArrayClass::Initialize(ID3D11Device *device, int fileNum, WCHAR **filenames)
{
	m_fileNum = fileNum;
	m_textures = new ID3D11ShaderResourceView*[fileNum];
	for (int i = 0; i < m_fileNum; i++)
	{
		if (FAILED(CreateDDSTextureFromFile(device, filenames[i], nullptr, &m_textures[i])))
		{
			return false;
		}
	}
	return true;
}

void TextureArrayClass::Shutdown()
{
	for (int i = 0; i < m_fileNum; i++)
	{
		if (m_textures[i])
		{
			m_textures[i]->Release();
			m_textures[i] = 0;
		}
	}

	delete[] m_textures;
}

ID3D11ShaderResourceView ** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}
