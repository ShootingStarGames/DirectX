#pragma once

class FPSClass
{
public:
	FPSClass();
	FPSClass(const FPSClass&);
	~FPSClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

