#pragma once
class ShaderClass
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

protected:
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	void ShutdownShader();
	void RenderShader(ID3D11DeviceContext*, int);

protected:
	ID3D11VertexShader * m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
};

