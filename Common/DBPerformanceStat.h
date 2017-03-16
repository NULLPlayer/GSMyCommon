#ifndef _DB_PERFORMANCE_STAT_H_
#define _DB_PERFORMANCE_STAT_H_

#include<string>
#include <map>
#include "GSTime.h"
#include "GSMutex.h"


class CDBPerformanceStat
{
protected:
	typedef struct _StruStatInfo
	{
		UInt64 maxTime;		//�����ʱ��
		UInt64 minTime;     //��С����ʱ��
		UInt64 totalTime;  
		UInt64 totalCount;	//�ܴ���
		UInt64 failCount;   //ʧ�ܵĴ���
		bool   bBeginStat;

		_StruStatInfo(void) : maxTime(0), minTime(0), totalTime(0), totalCount(0), failCount(0), bBeginStat(true) {};

	}StruStatInfo;

	typedef std::map<std::string, StruStatInfo> StatInfoMap;
	typedef StatInfoMap::iterator               StatInfoIter;
	typedef StatInfoMap::value_type             StatInfoValue;
public:
	CDBPerformanceStat(const std::string & strStatObjRemark = "DBPerformanceStat");
	virtual ~CDBPerformanceStat(void);

	void AddAStat(const std::string & strKey, UInt64 nTime);	//���һ��ͳ��

	void AddAFailStat(const std::string & strKey);

	//��ȡͳ����Ϣ
	void GetInfo(std::string & strInfo);

protected:
	GSMutex      m_mutexLock;
	StatInfoMap  m_mapStatInfo;
	std::string  m_strStatObjRemark;
};

#endif
