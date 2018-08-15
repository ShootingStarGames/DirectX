#pragma once

#include <Pdh.h>

class CPUClass
{
public:
	CPUClass();
	CPUClass(const CPUClass&);
	~CPUClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCPUPercentage();

private:
	bool m_canReadCpu = true;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime = 0;
	long m_cpuUsage = 0;
};

