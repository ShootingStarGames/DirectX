#pragma once
class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, int, WCHAR**);
	void Shutdown();

	int GetTextureNum();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	int m_fileNum = 0;
	ID3D11ShaderResourceView ** m_textures;
};

