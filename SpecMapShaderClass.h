#pragma once
class SpecMapShaderClass : ShaderClass
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT4 specularColor;
		float specularPower;
		XMFLOAT3 lightDirection;
	};
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};
public:
	SpecMapShaderClass();
	SpecMapShaderClass(const SpecMapShaderClass&);
	~SpecMapShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, int, ID3D11ShaderResourceView**,
		XMFLOAT3, XMFLOAT4, XMFLOAT3, XMFLOAT4, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	HRESULT InitializeLayout(ID3D11Device*, ID3D10Blob*) override;
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, int, ID3D11ShaderResourceView**,
		XMFLOAT3, XMFLOAT4, XMFLOAT3, XMFLOAT4, float);

private:
	ID3D11Buffer *m_lightBuffer = nullptr;
	ID3D11Buffer *m_cameraBuffer = nullptr;
};
