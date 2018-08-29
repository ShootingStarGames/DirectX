#pragma once
class BaseBuffer
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
public:
	BaseBuffer();
	BaseBuffer(const BaseBuffer&);
	~BaseBuffer();
};

