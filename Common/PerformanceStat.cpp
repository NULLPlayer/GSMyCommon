#include "PerformanceStat.h"
#include <sstream>

CPerformanceStat::CPerformanceStat(const std::string& statObjRemark):m_statObjRemark(statObjRemark)
{}

CPerformanceStat::~CPerformanceStat()
{}

void CPerformanceStat::BeginAStat()//��ʼһ��ͳ��
{
	m_timeMeter.Reset();
}

UInt64 CPerformanceStat::EndAStat()//����һ��ͳ��
{
	UInt64 time = m_timeMeter.GetElapsed();
	InnerAddAStat(time);
	return time;
}

void CPerformanceStat::AddAFailStat()
{
	GSAutoMutex csAuto(m_mutexLock);
	m_StatInfo.nFailNumStat ++;	
}

void CPerformanceStat::AddAStat(UInt64 time)
{
	GSAutoMutex csAuto(m_mutexLock);
	InnerAddAStat(time);
}

void CPerformanceStat::InnerAddAStat(UInt64 time)
{
	m_StatInfo.count ++;
	m_StatInfo.totaltime += time;

	if(time > m_StatInfo.maxTime)
		m_StatInfo.maxTime = time;
	if(time < m_StatInfo.minTime)
		m_StatInfo.minTime = time;
}

void CPerformanceStat::GetInfo(std::string& info)
{
	GSAutoMutex csAuto(m_mutexLock);

	if(m_StatInfo.count > 0) {
		std::stringstream statinfo;
		statinfo << "�������:" << m_StatInfo.count << " ----ƽ������ʱ��:"
			<< m_StatInfo.totaltime / (double)m_StatInfo.count << "ms ----�����ʱ��:" << m_StatInfo.maxTime
			<< "ms ----��С����ʱ��:" << m_StatInfo.minTime << "ms ----ʧ�ܴ���:" << m_StatInfo.nFailNumStat;
		info = statinfo.str();
	} else {
		info = "��ͳ����Ϣ";
	}
}

void CPerformanceStat::Reset()
{
	GSAutoMutex csAuto(m_mutexLock);
	m_StatInfo.Init();
}
