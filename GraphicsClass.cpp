#include "stdafx.h"
#include "D3DClass.h"
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
	d3d = new D3DClass;
	if (!d3d)
	{
		return false;
	}
	if (!d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialze Direct3D.", L"Error", MB_OK);
	}
	return false;
}


void GraphicsClass::Shutdown()
{
	if (d3d)
	{
		d3d->Shutdown();
		delete d3d;
		d3d = 0;
	}
}


bool GraphicsClass::Frame()
{
	return Render();
}


bool GraphicsClass::Render()
{
	//버퍼 삭제
	d3d->BegineScene(0.5f, 0.5f, 0.5f, 0.5f);

	//화면 출력 
	d3d->EndScene();
	return false;
}
