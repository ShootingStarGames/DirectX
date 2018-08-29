#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "lightshaderclass.h"
#include "TextureShaderClass.h"
#include "BitmapClass.h"
#include "TextClass.h"
#include "ModelListClass.h"
#include "FrustumClass.h"
#include "MultiTextureShaderClass.h"
#include "BumpMapShaderClass.h"
#include "SpecMapShaderClass.h"
#include "RenderTextureClass.h"
#include "DebugWindowClass.h"
#include "FogShaderClass.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	{
		// Direct3D 객체 생성
		m_Direct3D = new D3DClass;
		if (!m_Direct3D)
		{
			return false;
		}

		// Direct3D 객체 초기화
		if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
		{
			MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
			return false;
		}
	}
	{
		// m_Camera 객체 생성
		m_Camera = new CameraClass;
		if (!m_Camera)
		{
			return false;
		}

		// 카메라 포지션 설정
		XMMATRIX baseViewMatrix;
		m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
		m_Camera->Render();
		m_Camera->GetViewMatrix(baseViewMatrix);
	}
	{
		// m_Model 객체 생성
		m_Model = new ModelClass;
		if (!m_Model)
		{
			return false;
		}

		WCHAR* textureArray[1] { L"./data/seafloor.dds" };
		// m_Model 객체 초기화
		if (!m_Model->Initialize(m_Direct3D->GetDevice(), "./data/cube.txt", 1, textureArray))
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}
	{
		m_RenderTexture = new RenderTextureClass;
		if (!m_RenderTexture)
		{
			return false;
		}
		if (!m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight))
		{
			return false;
		}
	}
	{
		m_DebugWindow = new DebugWindowClass;
		if (!m_DebugWindow)
		{
			return false;
		}
		if (!m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 100, 100))
		{
			return false;
		}
	}
	{
		m_TextureShader = new TextureShaderClass;
		if (!m_TextureShader)
		{
			return false;
		}
		if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
		{
			return false;
		}
	}
	{
		m_FogShader = new FogShaderClass;
		if (!m_FogShader)
		{
			return false;
		}
		if (!m_FogShader->Initialize(m_Direct3D->GetDevice(), hwnd))
		{
			return false;
		}
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	if (m_SpecMapShader)
	{
		m_SpecMapShader->Shutdown();
		delete m_SpecMapShader;
		m_SpecMapShader = 0;
	}
	if (m_BumpMapShader)
	{
		m_BumpMapShader->Shutdown();
		delete m_BumpMapShader;
		m_BumpMapShader = 0;
	}
	if (m_MultiTextureShader)
	{
		m_MultiTextureShader->Shutdown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = 0;
	}
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}
	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	// m_Light 객체 반환
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// m_LightShader 객체 반환
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// m_Model 객체 반환
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Camera 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	return Render();
}

bool GraphicsClass::Render()
{
	if (!RenderToTexture())
	{
		return false;
	}
	float fogColor = .5f;
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	if (!RenderScene())
	{
		return false;
	}

	m_Direct3D->TurnZBufferOff();
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	if (!m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 50, 50))
	{
		return false;
	}

	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_RenderTexture->GetShaderResourceView()))
	{
		return false;
	}
	
	m_Direct3D->TurnZBufferOn();

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture()
{
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 1.0f, 1.0f, 1.0f, 1.0f);

	if (!RenderScene())
	{
		return false;
	}

	m_Direct3D->SetBackBufferRenderTarget();
	return true;
}

bool GraphicsClass::RenderScene()
{
	m_Camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	worldMatrix = XMMatrixRotationY(rotation);

	m_Model->Render(m_Direct3D->GetDeviceContext());


	float fogStart = 0.0f;
	float fogEnd = 5.0f;

	return m_FogShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureNum(), m_Model->GetTextureArray()
		, fogStart, fogEnd);
}
