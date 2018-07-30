#include "stdafx.h"
#include "TextureClass.h"
#include "FontClass.h"

#include <fstream>
using namespace std;

FontClass::FontClass()
{
}

FontClass::FontClass(const FontClass &)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device *device, char *fontFilename, WCHAR *textureFilename)
{	
	if (!LoadFontData(fontFilename))
	{
		return false;
	}
	return LoadTexture(device, textureFilename);
}

void FontClass::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();
}

ID3D11ShaderResourceView * FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void *vertices, char *sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + 0.3f;
		}
		else
		{

		}
	}
}

bool FontClass::LoadFontData(char *filename)
{
	ifstream fin;
	int i;
	char temp;

	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	for (int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = nullptr;
	}
}

bool FontClass::LoadTexture(ID3D11Device *device, WCHAR *filename)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	return m_Texture->Initialize(device, filename);
}

void FontClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}
}
