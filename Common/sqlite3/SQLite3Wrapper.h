/********************************************************************
	created:	2015/07/31
	created:	31:7:2015   19:15
	filename: 	SQLite3Wrapper.h
	file base:	SQLite3Wrapper
	file ext:	h
	author:		guanxj
	
	purpose:	sqlite3 API��װ��
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
	TYPE_TEXT,		 // UTF-8������ı�	
	TYPE_TEXT16,     // UTF-16������ı�
	TYPE_BLOB,
}EnumSQLLite3DataType;


// ���ݼ�
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
	* desc       : �Ƿ��¼��β��      
	* input      : NA  
	* output     : NA  
	* return     : false�����Ǽ�¼��β����true���Ǽ�¼��β��  
	***********************************************************************************/
	bool Eof(void);

	/***********************************************************************************  
	* name       : MoveNext  
	* desc       : ������һ����¼     
	* input      : NA  
	* output     : NA  
	* return     : ����ֵ: true �ɹ�  false
	***********************************************************************************/
	//bool MoveNext(void);

	/***********************************************************************************  
	* name       : GetColumns  
	* desc       : ��ȡ����    
	* input      : NA  
	* output     : NA  
	* return     : ���� 
	***********************************************************************************/
	unsigned int GetColumns(void);

	/***********************************************************************************  
	* name       : GetColumnName  
	* desc       : ��ȡ������
	* input      : iColIndex��������  
	* output     : NA  
	* return     : ����
	***********************************************************************************/
	const char* GetColumnName(unsigned int iColIndex);

	/***********************************************************************************  
	* name       : GetValue  
	* desc       : ��ȡ�ֶ�ֵ 
	* input      : szFieldName���ֶ����� 
	* input      : iBufSize��   Buffer��С
	* input      : eType��      �ֶ�����
	* output     : pBuffer��    �ֶ�ֵ������
	* output     : iFactLen��   ʵ�ʳ���
	* return     : ture���ɹ���false��ʧ��
	***********************************************************************************/
	bool GetValue(const char* szFieldName, 
		void* pBuffer,
		unsigned int iBufSize,
		unsigned int* iFactLen,
		EnumSQLLite3DataType eType);


	/***********************************************************************************  
	* name       : BindField  
	* desc       : ���ֶε���Ϣ 
	* input      : iRowIndex���ֶ����� ����1��ʼ
	* input      : eType��    �ֶ�����
	* input      : pBuffer��  ��Ҫ�󶨵��ֶ�ֵ
	* output     : iBufSize�� �����ݵĳ��ȣ���������������ʱ��Ҫָ����
	* output     : bNull��    �Ƿ�Ϊ��
	* return     : ture���ɹ���false��ʧ��
	***********************************************************************************/
	bool BindField(unsigned int iRowIndex, 
		EnumSQLLite3DataType eType, 
		void *pBuffer, 
		unsigned int iBufSize = 0, 
		bool bNull = false);


	/***********************************************************************************  
	* name       : ExecuteBind  
	* desc       : ִ�а�(�ǲ�ѯ)
	* input      : NA
    * output     : NA
	* return     : ture���ɹ���false��ʧ��
	***********************************************************************************/
	bool ExecuteBind(void);

	/***********************************************************************************  
	* name       : ResetBindParam  
	* desc       : ���³�ʼ���󶨲��� 
	* input      : NA
    * output     : NA
	* return     : NA
	***********************************************************************************/
	void ResetBindParam(void);

private:
	bool DataTypeMatch(unsigned int iColumn, EnumSQLLite3DataType eType);
	bool GetFieldList(void);
	bool GetFieldIndex(const char* szFieldName, unsigned int& iIndex);   //�ж��Ƿ�Ϊ���һ���ֶ�

private:
	sqlite3_stmt* m_pStmt;
	sqlite3*      m_pSqlite;
	std::map<std::string, unsigned int>  m_mapFieldList;     //��¼�ֶ��������ֶ�λ��
};

class SQLiteIConnection
{
public:
	SQLiteIConnection(void) : m_pSqlite(NULL) {}
	virtual ~SQLiteIConnection(void){}

public:
	/***********************************************************************************  
	* name       : Open  
	* desc       : ��sqlite   
	* input      : strFileName�����ݿ�����  
	* output     : NA  
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	bool Open(const std::string & strFileName);


	/***********************************************************************************  
	* name       : IsOpen  
	* desc       : �Ƿ�sqlite   
	* input      : NA  
	* output     : NA  
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	bool IsOpen(void);

	/***********************************************************************************  
	* name       : Close  
	* desc       : �ر�sqllite  
	* input      : NA  
	* output     : NA  
	* return     : NA
	***********************************************************************************/
	void Close(void);


	/***********************************************************************************  
	* name       : ExecuteSql  
	* desc       : Sqliteִ��sql 
	* input      : strSql��sql�ַ���  
	* output     : NA  
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	bool ExecuteSql(const std::string & strSql);

	/***********************************************************************************  
	* name       : ExecuteSqlRetID  
	* desc       : Sqliteִ��sql�����룩
	* input      : strSql��sql�ַ���  
	* output     : NA  
	* return     : ��¼������ID
	***********************************************************************************/
	Int64 ExecuteSqlRetID(const std::string & strSql);

	/***********************************************************************************  
	* name       : PrepareRecordSet  
	* desc       : ׼����¼��
	* input      : strSql��sql�ַ���  
	* output     : NA  
	* return     : SQLiteIRecordSetָ��
	***********************************************************************************/
	SQLiteIRecordSet* PrepareRecordSet(const std::string & strSql);

	/***********************************************************************************  
	* name       : ReleaseRecordSet  
	* desc       : �ͷ����ݼ�
	* input      : NA  
	* output     : NA  
	* return     : NA
	***********************************************************************************/
	void ReleaseRecordSet(SQLiteIRecordSet* pIRecordSet);

	/***********************************************************************************  
	* name       : ExecuteQuery  
	* desc       : Sqlliteִ�в�ѯ
	* input      : strSql��sql�ַ���
	* output     : strResultList�������¼��
	* output     : iRows��        ������
	* output     : iCols��        ������ 
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	bool ExecuteQuery(const std::string & strSql, STRINGLIST & strResultList, int & iRows, int & iCols);

	/***********************************************************************************  
	* name       : ExecutePageQuery  
	* desc       : ִ�з�ҳ��ѯsql,�������ݼ�
	* input      : strSql��   sql�ַ���
	* output     : iStartRow����ʼ��
	* output     : iRowNum��  ��������
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	SQLiteIRecordSet* ExecutePageQuery(const std::string & strSql, int iStartRow, int iRowNum);	

	/***********************************************************************************  
	* name       : BeginTrans 
	* desc       : �������� 
	* input      : NA  
	* output     : NA  
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	bool BeginTrans(void);

	/***********************************************************************************  
	* name       : Rollback 
	* desc       : �ع����� 
	* input      : NA  
	* output     : NA  
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	bool Rollback(void);

	/***********************************************************************************  
	* name       : Commit 
	* desc       : �ύ����
	* input      : NA  
	* output     : NA  
	* return     : ture���ɹ���false��ʧ�� 
	***********************************************************************************/
	bool Commit(void);

	/***********************************************************************************  
	* name       : GetLastErrMsg 
	* desc       : ��ȡ��������Ϣ
	* input      : NA  
	* output     : NA  
	* return     : ������Ϣ 
	***********************************************************************************/
	const char* GetLastErrMsg(void);

	/***********************************************************************************  
	* name       : GetLastErrCode 
	* desc       : ��ȡ�������
	* input      : NA  
	* output     : NA  
	* return     : ����� 
	***********************************************************************************/
	int GetLastErrCode(void);

	sqlite3*	GetSqlitePtr();

private:
	sqlite3*          m_pSqlite;
	//SQLiteIRecordSet* m_pIRecordSet;
	// ʱ�仺����
	//char			  m_szDateTime[256];
};

#endif


