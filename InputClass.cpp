#include "stdafx.h"
#include "InputClass.h"


InputClass::InputClass()
{
	for (int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}
}

InputClass::InputClass(const InputClass &)
{
}


InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
}

void InputClass::KeyDown(UINT input)
{
	keys[input] = true;
}

void InputClass::KeyUp(UINT input)
{
	keys[input] = false;
}

bool InputClass::IsKeyDown(UINT input)
{
	return keys[input];
}
