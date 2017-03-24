#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include "CommonDef.h"

class CSystemInfo
{
public:
	CSystemInfo(void);
	~CSystemInfo(void);

	GSString GetCPUVersion(void);
	GSString GetOSVersion(void);

	unsigned long long GetCurProcessMemoryWrokingSet(void);
	unsigned long long GetCurProcessMemoryPeakWorkingSet(void);
	unsigned long long GetTotalPhysicalMemory(void);
	int GetMemoryUsage(void);

	double GetCpuUsage(void);
	double GetCurProcessCpuUsage(void);

	bool GetCurProcessDiskIOBytes(unsigned long long* read_bytes, unsigned long long* write_bytes);
};

#endif // SYSTEM_INFO_H