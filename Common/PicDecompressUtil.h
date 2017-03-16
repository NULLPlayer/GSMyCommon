#ifndef _PICDECOMPRESSUTIL_H_
#define _PICDECOMPRESSUTIL_H_

#include <sstream>

#include "LibPicHandleManage.hpp"
#include "GSStrUtil.hpp"
#include "libPicture.h"
#include "pluginInfo.h"
#include "GSMemoryPool.h"
#include "Singleton.hpp"
#include "PerformanceStat.h"

class CPicDecompressUtil
{
public:
	CPicDecompressUtil(EnumGSPixFormat targetPixFormat = GS_PIX_NONE);
	~CPicDecompressUtil(void);

	static void InitForLog(SystemInfo* pSys);
	static SystemInfo* GetSystemInfo();

	static EnumGSPictureType GetPicTypeFromSuffix(const GSString& strPicNameWithSuffix);
	void InitTargetPixFormat(EnumGSPixFormat targetPixFormat);
	Int32 DecodePicture(EnumGSPictureType ePicType, const void* pBuffer, Int32 nSize, void** pDecodeBuffer, Int32* pDecodeSize, Int32* nOutWidth, Int32* nOutHeight);
	void FreeDecodeMem(void* pData);

private:
	static void* AllocPicMemCB(int nsize, void *userdata);

private:
	static SystemInfo m_system;
	EnumGSPixFormat m_targetPixFormat;

	CLibPicDecHandleMgr m_libPicDecHandleMgr;
	GSMemoryPool* m_pPicDecMemPool;

public:
	// perf stat
	CPerformanceStat m_decodePerfStat;
};

#define UTIL_LOG(msg, level) do { \
	SystemInfo* pSysInfo = INSTANCE(CPicDecompressUtil).GetSystemInfo(); \
	if (pSysInfo && pSysInfo->log) { \
		std::ostringstream oss; \
		oss << __FUNCTION__ << "() " << msg; \
		std::string strLog = oss.str();	\
		pSysInfo->log(strLog.c_str(), level, true); \
	} \
}while(0)

#endif // _PICDECOMPRESSUTIL_H_