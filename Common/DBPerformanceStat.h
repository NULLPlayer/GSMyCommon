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
		UInt64 maxTime;		//最大处理时间
		UInt64 minTime;     //最小处理时间
		UInt64 totalTime;  
		UInt64 totalCount;	//总次数
		UInt64 failCount;   //失败的次数
		bool   bBeginStat;

		_StruStatInfo(void) : maxTime(0), minTime(0), totalTime(0), totalCount(0), failCount(0), bBeginStat(true) {};

	}StruStatInfo;

	typedef std::map<std::string, StruStatInfo> StatInfoMap;
	typedef StatInfoMap::iterator               StatInfoIter;
	typedef StatInfoMap::value_type             StatInfoValue;
public:
	CDBPerformanceStat(const std::string & strStatObjRemark = "DBPerformanceStat");
	virtual ~CDBPerformanceStat(void);

	void AddAStat(const std::string & strKey, UInt64 nTime);	//添加一次统计

	void AddAFailStat(const std::string & strKey);

	//获取统计信息
	void GetInfo(std::string & strInfo);

protected:
	GSMutex      m_mutexLock;
	StatInfoMap  m_mapStatInfo;
	std::string  m_strStatObjRemark;
};

#endif
