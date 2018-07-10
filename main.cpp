#include "stdafx.h"
#include "SystemClass.h"

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hprevInstance,
	_In_ PSTR IpCmdLine,
	_In_ int CmdShow)
{
	SystemClass* System = new SystemClass;
	
	if (!System)
	{
		return - 1;
	}

	if (System->Initialize())
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}