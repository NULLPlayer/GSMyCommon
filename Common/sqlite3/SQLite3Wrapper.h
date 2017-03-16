/********************************************************************
	created:	2015/07/31
	created:	31:7:2015   19:15
	filename: 	SQLite3Wrapper.h
	file base:	SQLite3Wrapper
	file ext:	h
	author:		guanxj
	
	purpose:	sqlite3 API封装类
*********************************************************************/
#ifndef _SQLITE3_WRAPPER_H_
#define _SQLITE3_WRAPPER_H_
#include "sqlite3.h"
#include "GSTypes.h"
#include <string>
#include <vector>
#include <map>

typedef std::vector<std::string> STRINGLIST;

typedef enum 
{
	TYPE_DATA_UNKNOWN = 0,
	TYPE_INT32,
	TYPE_INT64,
	TYPE_DOUBLE,
	TYPE_TEXT,		 // UTF-8编码的文本	
	TYPE_TEXT16,     // UTF-16编码的文本
	TYPE_BLOB,
}EnumSQLLite3DataType;


// 数据集
class SQLiteIRecordSet
{
public:
	SQLiteIRecordSet(sqlite3* pSqlite3, sqlite3_stmt* pStmt) : m_pSqlite(pSqlite3), m_pStmt(pStmt){};
	virtual ~SQLiteIRecordSet(void){
		if (m_pStmt)
		{
			sqlite3_finalize(m_pStmt);
			m_pStmt = NULL;
		}
	};

public:
	/***********************************************************************************  
	* name       : Eof  
	* desc       : 是否记录集尾部      
	* input      : NA  
	* output     : NA  
	* return     : false：不是记录集尾部；true：是记录集尾部  
	***********************************************************************************/
	bool Eof(void);

	/***********************************************************************************  
	* name       : MoveNext  
	* desc       : 移向下一条记录     
	* input      : NA  
	* output     : NA  
	* return     : 返回值: true 成功  false
	***********************************************************************************/
	//bool MoveNext(void);

	/***********************************************************************************  
	* name       : GetColumns  
	* desc       : 获取列数    
	* input      : NA  
	* output     : NA  
	* return     : 列数 
	***********************************************************************************/
	unsigned int GetColumns(void);

	/***********************************************************************************  
	* name       : GetColumnName  
	* desc       : 获取列名称
	* input      : iColIndex：列索引  
	* output     : NA  
	* return     : 列名
	***********************************************************************************/
	const char* GetColumnName(unsigned int iColIndex);

	/***********************************************************************************  
	* name       : GetValue  
	* desc       : 获取字段值 
	* input      : szFieldName：字段名称 
	* input      : iBufSize：   Buffer大小
	* input      : eType：      字段类型
	* output     : pBuffer：    字段值缓存区
	* output     : iFactLen：   实际长度
	* return     : ture：成功；false：失败
	***********************************************************************************/
	bool GetValue(const char* szFieldName, 
		void* pBuffer,
		unsigned int iBufSize,
		unsigned int* iFactLen,
		EnumSQLLite3DataType eType);


	/***********************************************************************************  
	* name       : BindField  
	* desc       : 绑定字段的信息 
	* input      : iRowIndex：字段索引 ；由1开始
	* input      : eType：    字段类型
	* input      : pBuffer：  需要绑定的字段值
	* output     : iBufSize： 绑定内容的长度（二进制数据类型时需要指定）
	* output     : bNull：    是否为空
	* return     : ture：成功；false：失败
	***********************************************************************************/
	bool BindField(unsigned int iRowIndex, 
		EnumSQLLite3DataType eType, 
		void *pBuffer, 
		unsigned int iBufSize = 0, 
		bool bNull = false);


	/***********************************************************************************  
	* name       : ExecuteBind  
	* desc       : 执行绑定(非查询)
	* input      : NA
    * output     : NA
	* return     : ture：成功；false：失败
	***********************************************************************************/
	bool ExecuteBind(void);

	/***********************************************************************************  
	* name       : ResetBindParam  
	* desc       : 重新初始化绑定参数 
	* input      : NA
    * output     : NA
	* return     : NA
	***********************************************************************************/
	void ResetBindParam(void);

private:
	bool DataTypeMatch(unsigned int iColumn, EnumSQLLite3DataType eType);
	bool GetFieldList(void);
	bool GetFieldIndex(const char* szFieldName, unsigned int& iIndex);   //判断是否为表的一个字段

private:
	sqlite3_stmt* m_pStmt;
	sqlite3*      m_pSqlite;
	std::map<std::string, unsigned int>  m_mapFieldList;     //记录字段名字与字段位置
};

class SQLiteIConnection
{
public:
	SQLiteIConnection(void) : m_pSqlite(NULL) {}
	virtual ~SQLiteIConnection(void){}

public:
	/***********************************************************************************  
	* name       : Open  
	* desc       : 打开sqlite   
	* input      : strFileName：数据库名称  
	* output     : NA  
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	bool Open(const std::string & strFileName);


	/***********************************************************************************  
	* name       : IsOpen  
	* desc       : 是否sqlite   
	* input      : NA  
	* output     : NA  
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	bool IsOpen(void);

	/***********************************************************************************  
	* name       : Close  
	* desc       : 关闭sqllite  
	* input      : NA  
	* output     : NA  
	* return     : NA
	***********************************************************************************/
	void Close(void);


	/***********************************************************************************  
	* name       : ExecuteSql  
	* desc       : Sqlite执行sql 
	* input      : strSql：sql字符串  
	* output     : NA  
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	bool ExecuteSql(const std::string & strSql);

	/***********************************************************************************  
	* name       : ExecuteSqlRetID  
	* desc       : Sqlite执行sql（插入）
	* input      : strSql：sql字符串  
	* output     : NA  
	* return     : 记录的自增ID
	***********************************************************************************/
	Int64 ExecuteSqlRetID(const std::string & strSql);

	/***********************************************************************************  
	* name       : PrepareRecordSet  
	* desc       : 准备记录集
	* input      : strSql：sql字符串  
	* output     : NA  
	* return     : SQLiteIRecordSet指针
	***********************************************************************************/
	SQLiteIRecordSet* PrepareRecordSet(const std::string & strSql);

	/***********************************************************************************  
	* name       : ReleaseRecordSet  
	* desc       : 释放数据集
	* input      : NA  
	* output     : NA  
	* return     : NA
	***********************************************************************************/
	void ReleaseRecordSet(SQLiteIRecordSet* pIRecordSet);

	/***********************************************************************************  
	* name       : ExecuteQuery  
	* desc       : Sqllite执行查询
	* input      : strSql：sql字符串
	* output     : strResultList：结果记录集
	* output     : iRows：        行索引
	* output     : iCols：        列索引 
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	bool ExecuteQuery(const std::string & strSql, STRINGLIST & strResultList, int & iRows, int & iCols);

	/***********************************************************************************  
	* name       : ExecutePageQuery  
	* desc       : 执行分页查询sql,返回数据集
	* input      : strSql：   sql字符串
	* output     : iStartRow：起始行
	* output     : iRowNum：  返回行数
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	SQLiteIRecordSet* ExecutePageQuery(const std::string & strSql, int iStartRow, int iRowNum);	

	/***********************************************************************************  
	* name       : BeginTrans 
	* desc       : 开启事务 
	* input      : NA  
	* output     : NA  
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	bool BeginTrans(void);

	/***********************************************************************************  
	* name       : Rollback 
	* desc       : 回滚事务 
	* input      : NA  
	* output     : NA  
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	bool Rollback(void);

	/***********************************************************************************  
	* name       : Commit 
	* desc       : 提交事务
	* input      : NA  
	* output     : NA  
	* return     : ture：成功；false：失败 
	***********************************************************************************/
	bool Commit(void);

	/***********************************************************************************  
	* name       : GetLastErrMsg 
	* desc       : 获取最后错误信息
	* input      : NA  
	* output     : NA  
	* return     : 错误信息 
	***********************************************************************************/
	const char* GetLastErrMsg(void);

	/***********************************************************************************  
	* name       : GetLastErrCode 
	* desc       : 获取错误代号
	* input      : NA  
	* output     : NA  
	* return     : 错误号 
	***********************************************************************************/
	int GetLastErrCode(void);

	sqlite3*	GetSqlitePtr();

private:
	sqlite3*          m_pSqlite;
	//SQLiteIRecordSet* m_pIRecordSet;
	// 时间缓冲区
	//char			  m_szDateTime[256];
};

#endif


