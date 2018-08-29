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
		// Direct3D ��ü ����
		m_Direct3D = new D3DClass;
		if (!m_Direct3D)
		{
			return false;
		}

		// Direct3D ��ü �ʱ�ȭ
		if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
		{
			MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
			return false;
		}
	}
	{
		// m_Camera ��ü ����
		m_Camera = new CameraClass;
		if (!m_Camera)
		{
			return false;
		}

		// ī�޶� ������ ����
		XMMATRIX baseViewMatrix;
		m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
		m_Camera->Render();
		m_Camera->GetViewMatrix(baseViewMatrix);
	}
	{
		// m_Model ��ü ����
		m_Model = new ModelClass;
		if (!m_Model)
		{
			return false;
		}

		WCHAR* textureArray[3] { L"./data/stone02.dds",L"./data/bump02.dds" ,L"./data/spec02.dds" };
		// m_Model ��ü �ʱ�ȭ
		if (!m_Model->Initialize(m_Direct3D->GetDevice(), "./data/cube.txt", 3, textureArray))
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
			MessageBox(hwnd, L"Could not initialize the RenderTextureClass object.", L"Error", MB_OK);
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
			MessageBox(hwnd, L"Could not initialize the debugWindow object.", L"Error", MB_OK);
			return false;
		}
	}
	{
		m_SpecMapShader = new SpecMapShaderClass;

		if (!m_SpecMapShader)
		{
			return false;
		}

		if (!m_SpecMapShader->Initialize(m_Direct3D->GetDevice(), hwnd))
		{
			MessageBox(hwnd, L"Could not initialize the SpecMapShaderClass object.", L"Error", MB_OK);
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
			MessageBox(hwnd, L"Could not initialize the TextureShaderClass object.", L"Error", MB_OK);
			return false;
		}
	}
	{
		// m_Light ��ü ����
		m_Light = new LightClass;
		if (!m_Light)
		{
			return false;
		}

		// m_Light ��ü �ʱ�ȭ
		m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetDirection(0.0f, 0.0f, 1.0f);
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(16.0f);
	}
	return true;
}


void GraphicsClass::Shutdown()
{
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
	// m_Light ��ü ��ȯ
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// m_LightShader ��ü ��ȯ
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// m_Model ��ü ��ȯ
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Camera ��ü ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D ��ü ��ȯ
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
	// ���� �׸��� ���� ���۸� ����ϴ�
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

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

	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), 
		worldMatrix, viewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView()))
	{
		return false;
	}

	m_Direct3D->TurnZBufferOn();

	// ������ ������ ȭ�鿡 ����մϴ�
	m_Direct3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture()
{
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

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

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	worldMatrix = XMMatrixRotationY(rotation);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	return m_SpecMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureNum(), m_Model->GetTextureArray()
		, m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition()
		, m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
}
