#include "stdafx.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
#include "GraphicsClass.h"


GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth,int screenHeight,HWND hwnd)
{
	//DirectX Class
	d3d = new D3DClass;
	if (!d3d)
	{
		return false;
	}
	if (!d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialze Direct3D.", L"Error", MB_OK);
	}

	//Camera Class
	camera = new CameraClass;
	if (!camera)
	{
		return false;
	}
	camera->SetPosition(0.0f, 0.0f, 0.0f);
	//Model Class
	model = new ModelClass;
	if (!model->Initialize(d3d->GetDevice(), L"data/seafloor.dds"))
	{
		MessageBox(hwnd, L"Could not initialze Model.", L"Error", MB_OK);
		return false;
	}
	
	//LightShader Class
	lightShader = new LightShaderClass;
	if (!lightShader->Initialize(d3d->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialze LightShader.", L"Error", MB_OK);
		return false;
	}

	//Light Class
	light = new LightClass;
	if (!light)
	{
		return false;
	}

	light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);
	return true;
}


void GraphicsClass::Shutdown()
{
	if (light)
	{
		delete light;
		light = 0;
	}

	if (lightShader)
	{
		lightShader->Shutdown();
		delete lightShader;
		lightShader = 0;
	}
	
	if (model)
	{
		model->Shutdown();
		delete model;
		model = 0;
	}

	if (camera)
	{
		delete camera;
		camera = 0;
	}

	if (d3d)
	{
		d3d->Shutdown();
		delete d3d;
		d3d = 0;
	}
}


bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;
	rotation += (float)XM_PI*0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	return Render(rotation);
}


bool GraphicsClass::Render(float rotation)
{
	//버퍼 삭제
	d3d->BegineScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	d3d->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	d3d->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(rotation);

	model->Render(d3d->GetDeviceContext());

	if (!lightShader->Render(d3d->GetDeviceContext(), model->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix, model->GetTexture(), light->GetDirection(), light->GetDiffuseColor())) {
		return false;
	}
	//화면 출력 
	d3d->EndScene();
	return false;
}
