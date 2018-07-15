#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class CameraClass;
class ModelClass;
class LightShaderClass;
class LightClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();
	bool Initialize(int,int,HWND);
	void Shutdown();
	bool Frame();
private:
	bool Render(float);
private:
	D3DClass* d3d = nullptr;
	CameraClass* camera = nullptr;
	ModelClass* model = nullptr;
	LightShaderClass* lightShader = nullptr;
	LightClass* light = nullptr;
};

