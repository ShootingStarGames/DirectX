#pragma once
class ModelListClass
{
private:
	struct ModelInfoType
	{
		XMFLOAT4 color;
		float positionX, positionY, positionZ;
	};
public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();

	bool Initializie(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, XMFLOAT4&);

private:
	int m_modelCount = 0;
	ModelInfoType* m_ModelInfoList = nullptr;
};

