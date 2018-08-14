#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "lightshaderclass.h"
#include "TextureShaderClass.h"
#include "BitmapClass.h"
#include "TextClass.h"
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

	

	m_TextureShader = new TextureShaderClass;m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix))
	{
		MessageBox(hwnd, L"Could not initialize the Text object", L"Error", MB_OK);
		return false;
	}

	if (!m_TextureShader)
	{
		return false;
	}

	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object", L"Error", MB_OK);
		return false;
	}

	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"./data/seafloor.dds", 256, 256))
	{
		MessageBox(hwnd, L"Could not initialize the Bitmap object", L"Error", MB_OK);
		return false;
	}
	// m_Model 객체 생성
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// m_Model 객체 초기화
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), "./data/cube.txt", L"./data/seafloor.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// m_LightShader 객체 생성
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// m_LightShader 객체 초기화
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// m_Light 객체 생성
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// m_Light 객체 초기화
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);


	return true;
}


void GraphicsClass::Shutdown()
{
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


bool GraphicsClass::Frame(int mouseX,int mouseY)
{
	static float rotation = 0.0f;

	// 각 프레임의 rotation 변수를 업데이트합니다.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	if (!m_Text->SetMousePosition(mouseX, mouseY, m_Direct3D->GetDeviceContext()))
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	// 그래픽 랜더링 처리
	return Render(rotation);
}


bool GraphicsClass::Render(float rotation)
{
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 1.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// 삼각형이 회전 할 수 있도록 회전 값으로 월드 행렬을 회전합니다.
	//worldMatrix = XMMatrixRotationY(rotation);

	m_Direct3D->TurnZBufferOff();

	m_Direct3D->TurnOnAlphaBlending();

	if (!m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix))
	{
		return false;
	}

	m_Direct3D->TurnOffAlphaBlending();
	if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 100, 100))
	{
		return false;
	}

	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture()))
	{
		return false;
	}

	m_Direct3D->TurnZBufferOn();
	//// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 드로잉을 준비합니다.
	//m_Model->Render(m_Direct3D->GetDeviceContext());

	//// Light 쉐이더를 사용하여 모델을 렌더링합니다.
	//if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
	//{
	//	return false;
	//}

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}