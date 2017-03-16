#include "LocalFileManage.h"
#include "MessageHandler.h"

#include "GSFile.h"

using namespace std;

CLocalFileManage::CLocalFileManage( SystemInfo* pSys, const GSString& strRootDir )
	: m_strRootDir(strRootDir)
{
	if (pSys == NULL) {
		GSAbort(0);
	}
	m_stSysInfo = *pSys;
}

CLocalFileManage::~CLocalFileManage( void )
{
}

bool CLocalFileManage::Start( void )
{
	if (!m_gsThread.Start(OnThreadCB, this)) {
		m_stSysInfo.log("GSThread Start Fail.", LOGGER_LEVEL_ERROR, true);
		return false;
	}

	//GSFetchDir(m_strRootDir.c_str(), OnFileCB, this);
	return true;
}

void CLocalFileManage::Stop( void )
{
	m_gsThread.Stop();
	m_gsThread.Join();
}

bool CLocalFileManage::GetNewFilePath( GSString& path )
{
	GSAutoMutex csAuto(m_mutex);
	return InnerPopPathListElem(path);
}

void CLocalFileManage::SwapNewFilePathList( std::list< GSString >& listPath )
{
	GSAutoMutex csAuto(m_mutex);
	InnerSwapPathList(listPath);
}

void CLocalFileManage::EraseFilePath( const GSString& path )
{
	GSString strLog;
	m_mutex.Lock();
	GSStrUtil::Format(strLog, "%s Erase File:%s", __FUNCTION__, path.c_str());
	m_stSysInfo.log(strLog.c_str(), LOGGER_LEVEL_DEBUG, true);
	InnerErasePathSetElem(path);
	m_mutex.Unlock();
}

#define INTERVAL_RECURDIR (1000)

void CLocalFileManage::OnThread( GSThread& thread )
{
	while (!thread.TestExit()) {
		GSFetchDir(m_strRootDir.c_str(), OnFileCB, this);
		Sleep(INTERVAL_RECURDIR);
	}
}

bool CLocalFileManage::OnFile( const GSStruFileInfo& info )
{
	//if (InnerGetPathListSize() > 1000) {
	//	return true;
	//}
	GSString strLog;
	switch (info.eType) {
	case GS_IFDIR: // dir
		if (info.szName) {
			GSFetchDir(info.szName, OnFileCB, this);
		} else {
			m_stSysInfo.log("dir name is NULL", LOGGER_LEVEL_ERROR, true);
		}
		break;
	case GS_IFREG: // file
		if (info.szName) {
			if (!FindPathSetElem(info.szName)) {
				GSStrUtil::Format(strLog, "%s Insert File:%s", __FUNCTION__, info.szName);
				m_stSysInfo.log(strLog.c_str(), LOGGER_LEVEL_DEBUG, true);
				InsertPathSetElem(info.szName);
				PushPathListElem(info.szName);
			}
		} else {
			m_stSysInfo.log("file name is NULL", LOGGER_LEVEL_ERROR, true);
		}
		break;
	default:
		break;
	}
	return true;
}

void CLocalFileManage::PushPathListElem( const GSString& path )
{
	m_mutex.Lock();
	InnerPushPathListElem(path);
	m_mutex.Unlock();
}

bool CLocalFileManage::FindPathSetElem( const GSString& path )
{
	GSAutoMutex csAuto(m_mutex);
	return InnerFindPathSetElem(path);
}

void CLocalFileManage::InsertPathSetElem( const GSString& path )
{
	m_mutex.Lock();
	InnerInsertPathSetElem(path);
	m_mutex.Unlock();
}

void CLocalFileManage::InnerPushPathListElem( const GSString& strPath )
{
	m_pathList.push_back(strPath);
}

bool CLocalFileManage::InnerPopPathListElem( GSString& strPath )
{
	if (m_pathList.empty()) {
		return false;
	}

	strPath = m_pathList.front();
	m_pathList.pop_front();
	return true;
}

void CLocalFileManage::InnerInsertPathSetElem( const GSString& strPath )
{
	m_pathSet.insert(strPath);
}

void CLocalFileManage::InnerErasePathSetElem( const GSString& strPath )
{
	m_pathSet.erase(strPath);
}

size_t CLocalFileManage::InnerGetPathListSize( void )
{
	return m_pathList.size();
}

void CLocalFileManage::InnerSwapPathList( std::list< GSString >& listPath )
{
	m_pathList.swap(listPath);
}

bool CLocalFileManage::InnerFindPathSetElem( const GSString& strPath )
{
	set< GSString >::iterator it = m_pathSet.find(strPath);
	if (it == m_pathSet.end()) {
		return false;
	}
	return true;
}
