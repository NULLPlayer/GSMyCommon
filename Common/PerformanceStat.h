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
		UInt64 count;			// 总次数
		UInt64 maxTime;			// 最大处理时间
		UInt64 minTime;			// 最小处理时间
		UInt64 totaltime;
		UInt64 nFailNumStat;	// 失败次数

		_StatInfo() { Init(); };
		void Init() { count = 0; maxTime = 0; minTime = (std::numeric_limits<UInt64>::max)(); totaltime = 0; nFailNumStat = 0; };
	} StatInfo;

public:
	CPerformanceStat(const std::string& statObjRemark); // 参数：统计对象标注,可自由加入相关字符描述统计对象
	~CPerformanceStat();

	void BeginAStat();   // 开始一次统计
	UInt64 EndAStat();   // 结束一次统计

	void AddAFailStat(); // 添加失败统计

	void AddAStat(UInt64 time); // 添加一次统计

	//获取统计信息
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
