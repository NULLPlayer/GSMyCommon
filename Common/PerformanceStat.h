#ifndef _PERFORMACNCESTAT_H_
#define _PERFORMACNCESTAT_H_

#include <string>
#include <limits>
#include "GSFoundation.h"

class CPerformanceStat
{
protected:
	typedef struct _StatInfo
	{
		UInt64 count;			// �ܴ���
		UInt64 maxTime;			// �����ʱ��
		UInt64 minTime;			// ��С����ʱ��
		UInt64 totaltime;
		UInt64 nFailNumStat;	// ʧ�ܴ���

		_StatInfo() { Init(); };
		void Init() { count = 0; maxTime = 0; minTime = (std::numeric_limits<UInt64>::max)(); totaltime = 0; nFailNumStat = 0; };
	} StatInfo;

public:
	CPerformanceStat(const std::string& statObjRemark); // ������ͳ�ƶ����ע,�����ɼ�������ַ�����ͳ�ƶ���
	~CPerformanceStat();

	void BeginAStat();   // ��ʼһ��ͳ��
	UInt64 EndAStat();   // ����һ��ͳ��

	void AddAFailStat(); // ���ʧ��ͳ��

	void AddAStat(UInt64 time); // ���һ��ͳ��

	//��ȡͳ����Ϣ
	virtual void GetInfo(std::string& info);

	void Reset();

private:
   void InnerAddAStat(UInt64 time);

private:
	std::string m_statObjRemark;
	GSTimeMeter m_timeMeter;

protected:
	StatInfo    m_StatInfo;
	GSMutex     m_mutexLock;
};


#endif
