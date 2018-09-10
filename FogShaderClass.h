#pragma once
class FogShaderClass : ShaderClass
{
private:
	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
	};
public:
	FogShaderClass();
	FogShaderClass(const FogShaderClass&);
	~FogShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, int, ID3D11ShaderResourceView**, float, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	HRESULT InitializeBuffer(ID3D11Device*,void*);
	HRESULT InitializeLayout(ID3D11Device*, ID3D10Blob*) override;
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, int, ID3D11ShaderResourceView**, float, float);

private:
	ID3D11Buffer * m_fogBuffer = nullptr;
};