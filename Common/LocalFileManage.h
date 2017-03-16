#ifndef LOCAL_FILE_MANAGE_H
#define LOCAL_FILE_MANAGE_H

#include "pluginInfo.h"
#include "GSSysApi.h"
#include "GSFoundation.h"

class CLocalFileManage
{
public:
	CLocalFileManage(SystemInfo* pSys, const GSString& strRootDir);
	~CLocalFileManage(void);

	bool Start(void);
	void Stop(void);

	bool GetNewFilePath(GSString& path);
	void SwapNewFilePathList(std::list< GSString >& listPath);
	void EraseFilePath(const GSString& path);

private:
	void OnThread(GSThread &thread);
	bool OnFile(const GSStruFileInfo &info);

	// 定时递归目录
	//static void OnTimerRecursiveDirectoryCB(SystemInfo* sys, TimerHandle timerID, void *pTimerParam) {
	//	GS_ASSERT_RET(pTimerParam);
	//	CLocalFileManage* pThis = (CLocalFileManage*) pTimerParam;
	//	pThis->OnTimerRecursiveDirectory();
	//}
	static void OnThreadCB(GSThread &thread, void *pThreadData) {
		GS_ASSERT_RET(pThreadData);
		CLocalFileManage* pThis = (CLocalFileManage*) pThreadData;
		pThis->OnThread(thread);
	}
	// GSFetchDir目录回调
	static bool OnFileCB(const GSStruFileInfo &info, void *pUserData) {
		GS_ASSERT_RET_VAL(pUserData, false);
		CLocalFileManage* pThis = (CLocalFileManage*) pUserData;
		return pThis->OnFile(info);
	}

private:
	void PushPathListElem(const GSString& path);
	bool FindPathSetElem(const GSString& path);
	void InsertPathSetElem(const GSString& path);

private:
	void InnerPushPathListElem(const GSString& strPath);
	bool InnerPopPathListElem(GSString& strPath);
	size_t InnerGetPathListSize(void);
	void InnerSwapPathList(std::list< GSString >& listPath);

	void InnerInsertPathSetElem(const GSString& strPath);
	void InnerErasePathSetElem(const GSString& strPath);
	bool InnerFindPathSetElem(const GSString& strPath);

private:
	SystemInfo m_stSysInfo;
	//TimerHandle m_hRecurDirTimer;
	GSThread m_gsThread;

	GSString m_strRootDir;

	GSMutex m_mutex;
	//std::list< GSString > m_dirList;
	std::list< GSString > m_pathList;
	std::set< GSString > m_pathSet;
};

#endif // LOCAL_FILE_MANAGE_H