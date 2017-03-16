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
		strInfo = "未有统计信息";
		return;
	}

	std::stringstream statinfo;
	for (StatInfoIter it = m_mapStatInfo.begin(); it != m_mapStatInfo.end(); it ++)
	{
		statinfo << it->first << " [--处理次数:" << it->second.totalCount << " --平均处理时间:" << it->second.totalTime/(double)it->second.totalCount
		<< "ms --失败的次数:" << it->second.failCount << " --最大时间:" << it->second.maxTime << "ms --最小时间:" << it->second.minTime << "ms]  \r\n";
	}

	strInfo = statinfo.str();
}
