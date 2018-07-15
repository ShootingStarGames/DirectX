#include "stdafx.h"
#include "LightClass.h"


LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass &)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	diffuseColor = XMFLOAT4(r, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return direction;
}
