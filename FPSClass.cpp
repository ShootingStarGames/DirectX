#include "stdafx.h"
#include "FPSClass.h"


FPSClass::FPSClass()
{
}

FPSClass::FPSClass(const FPSClass &)
{
}


FPSClass::~FPSClass()
{
}

void FPSClass::Initialize()
{
	m_fps = m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void FPSClass::Frame()
{
	m_count++;
	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		m_startTime = timeGetTime();
	}
}

int FPSClass::GetFps()
{
	return m_fps;
}
