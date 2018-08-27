#pragma once
class MultiTextureShaderClass : ShaderClass
{
public:
	MultiTextureShaderClass();
	MultiTextureShaderClass(const MultiTextureShaderClass&);
	~MultiTextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, int, ID3D11ShaderResourceView**);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX,int ,ID3D11ShaderResourceView**);
};

