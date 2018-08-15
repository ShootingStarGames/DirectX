#include "stdafx.h"
#include "PositionClass.h"


PositionClass::PositionClass()
{
}

PositionClass::PositionClass(const PositionClass &)
{
}


PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
}

void PositionClass::GetRotation(float &y)
{
	y = m_rotationY;
}

void PositionClass::TurnLeft(bool keyDown)
{
	if (keyDown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;
		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.05f;
		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}
}

void PositionClass::TurnRight(bool keyDown)
{
	if (keyDown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;
		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 0.05f;
		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}
}
