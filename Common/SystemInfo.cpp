#include "SystemInfo.h"
#include "GSSysApi.h"
#include <Windows.h>
#include <process.h>
#include <Psapi.h>

CSystemInfo::CSystemInfo(void)
{
}

CSystemInfo::~CSystemInfo(void)
{
}

GSString CSystemInfo::GetCPUVersion( void )
{
	const char* pCpuVersion = GSGetCPUVersion();
	if (pCpuVersion != NULL) {
		return pCpuVersion;
	}
	return GSString();
}

GSString CSystemInfo::GetOSVersion( void )
{
	const char* pOSVersion = GSGetOSVersion();
	if (pOSVersion != NULL) {
		return pOSVersion;
	}
	return GSString();
}

unsigned long long CSystemInfo::GetCurProcessMemoryWrokingSet( void )
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		return pmc.WorkingSetSize;
	}
	return 0;
}

unsigned long long CSystemInfo::GetCurProcessMemoryPeakWorkingSet( void )
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		return pmc.PeakWorkingSetSize;
	}
	return 0;
}

unsigned long long CSystemInfo::GetTotalPhysicalMemory( void )
{
	return GSGetTotalPhysicalMemory();
}

static double FileTimeToDouble(FILETIME* pFileTime) {
	return (double) ((*pFileTime).dwHighDateTime * 4.294967296E9/*1 << 32*/) + 
		(double) (*pFileTime).dwLowDateTime;
}

int CSystemInfo::GetMemoryUsage( void )
{
	MEMORYSTATUS ms;
	::GlobalMemoryStatus(&ms);
	return ms.dwMemoryLoad;
}

double CSystemInfo::GetCpuUsage( void )
{
	double nCpuUsage = -1;
	FILETIME ftIdle, ftKernel, ftUser;
	static double fCpuIdleTime = 0.0, fCpuKernelTime = 0.0, fCpuUserTime = 0.0;
	if (GetSystemTimes(&ftIdle, &ftKernel, &ftUser)) {
		double fNewCpuIdleTime = FileTimeToDouble(&ftIdle);
		double fNewCpuKernelTime = FileTimeToDouble(&ftKernel);
		double fNewCpuUserTime = FileTimeToDouble(&ftUser);

		nCpuUsage = 100.0 - ((fNewCpuIdleTime - fCpuIdleTime) * 100.0 / (fNewCpuKernelTime - fCpuKernelTime + fNewCpuUserTime - fCpuUserTime));

		fCpuIdleTime = fNewCpuIdleTime;
		fCpuKernelTime = fNewCpuKernelTime;
		fCpuUserTime = fNewCpuUserTime;
	}
	return nCpuUsage;
}

double CSystemInfo::GetCurProcessCpuUsage( void )
{
	return GSGetCurProcessCpuUsage();
}

bool CSystemInfo::GetCurProcessDiskIOBytes( unsigned long long* read_bytes, unsigned long long* write_bytes )
{
	IO_COUNTERS io;
	if (GetProcessIoCounters(GetCurrentProcess(), &io)) {
		if (read_bytes) *read_bytes = io.ReadTransferCount;
		if (write_bytes) *write_bytes = io.WriteTransferCount;
		return true;
	}
	return false;
}
