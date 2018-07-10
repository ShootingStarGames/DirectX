#include "stdafx.h"
#include "SystemClass.h"

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hprevInstance,
	_In_ LPWSTR IpCmdLine,
	_In_ int CmdShow)
{
	SystemClass *System = new SystemClass;
	
	if (!System)
	{
		return - 1;
	}


}