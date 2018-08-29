#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class ModelClass;
class LightClass;
class LightShaderClass;
class BitmapClass;
class TextureShaderClass;
class TextClass;
class ModelListClass;
class FrustumClass;
class MultiTextureShaderClass;
class BumpMapShaderClass;
class SpecMapShaderClass;
class RenderTextureClass;
class DebugWindowClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Render();

private:
	bool RenderToTexture();
	bool RenderScene();

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	LightShaderClass* m_LightShader = nullptr;
	LightClass* m_Light = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	BitmapClass* m_Bitmap = nullptr;
	TextClass* m_Text = nullptr;
	ModelListClass* m_ModelList = nullptr;
	FrustumClass* m_Frustum = nullptr;
	MultiTextureShaderClass* m_MultiTextureShader = nullptr;
	BumpMapShaderClass* m_BumpMapShader = nullptr;
	SpecMapShaderClass* m_SpecMapShader = nullptr;
	RenderTextureClass* m_RenderTexture = nullptr;
	DebugWindowClass* m_DebugWindow = nullptr;
};