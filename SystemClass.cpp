#include "stdafx.h"
#include "SystemClass.h"



SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass &)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialze()
{
	int screenWidth = 0;
	int screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);


	return false;
}

void SystemClass::Shutdown()
{
}

void SystemClass::Run()
{
}

LRESULT SystemClass::MessageHandler(HWND, UINT, WPARAM, LPARAM)
{
	return LRESULT();
}

bool SystemClass::Frame()
{
	return false;
}

void SystemClass::InitializeWindows(int &, int &)
{
}

void SystemClass::ShutdownWindows()
{
}
