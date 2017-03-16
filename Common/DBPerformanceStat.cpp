#include "DBPerformanceStat.h"
#include <sstream>


CDBPerformanceStat::CDBPerformanceStat(const std::string & strStatObjRemark) : m_strStatObjRemark(strStatObjRemark)
{
}

CDBPerformanceStat::~CDBPerformanceStat(void)
{
}


void CDBPerformanceStat::AddAStat(const std::string & strKey, UInt64 nTime)
{
	GSAutoMutex csAuto(m_mutexLock);

	m_mapStatInfo[strKey].totalCount += 1;
	m_mapStatInfo[strKey].totalTime  += nTime;

	if(m_mapStatInfo[strKey].bBeginStat)
	{
		m_mapStatInfo[strKey].bBeginStat = false;
		m_mapStatInfo[strKey].minTime = nTime;
		m_mapStatInfo[strKey].maxTime = nTime;
	}
	else
	{
		if(nTime > m_mapStatInfo[strKey].maxTime)
		{
			m_mapStatInfo[strKey].maxTime = nTime;
		}
		else 
		{
			if(nTime < m_mapStatInfo[strKey].minTime)
			{
				m_mapStatInfo[strKey].minTime = nTime;
			}
		}
	}
}


void CDBPerformanceStat::AddAFailStat(const std::string & strKey)
{
	GSAutoMutex csAuto(m_mutexLock);
	m_mapStatInfo[strKey].failCount += 1;
}


void CDBPerformanceStat::GetInfo(std::string & strInfo)
{
	if (m_mapStatInfo.empty())
	{
		strInfo = "δ��ͳ����Ϣ";
		return;
	}

	std::stringstream statinfo;
	for (StatInfoIter it = m_mapStatInfo.begin(); it != m_mapStatInfo.end(); it ++)
	{
		statinfo << it->first << " [--�������:" << it->second.totalCount << " --ƽ������ʱ��:" << it->second.totalTime/(double)it->second.totalCount
		<< "ms --ʧ�ܵĴ���:" << it->second.failCount << " --���ʱ��:" << it->second.maxTime << "ms --��Сʱ��:" << it->second.minTime << "ms]  \r\n";
	}

	strInfo = statinfo.str();
}
