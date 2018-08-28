#pragma once
class BumpMapShaderClass : ShaderClass
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
public:
	BumpMapShaderClass();
	BumpMapShaderClass(const BumpMapShaderClass&);
	~BumpMapShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, int, ID3D11ShaderResourceView**, XMFLOAT3, XMFLOAT4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	HRESULT InitializeLayout(ID3D11Device*, ID3D10Blob*) override;
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, int, ID3D11ShaderResourceView**, XMFLOAT3, XMFLOAT4);

private:
	ID3D11Buffer * m_lightBuffer = nullptr;
};
