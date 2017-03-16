#include "SQLite3Wrapper.h"
#include "GSDefs.h"
#include "GSStrUtil.hpp"
#include "util/StrCodeUtil.hpp"


bool SQLiteIRecordSet::Eof( void )
{   
	GS_ASSERT_RET_VAL(m_pStmt, false);
	if (sqlite3_step(m_pStmt) != SQLITE_ROW)
		return true;
	return false;
}


//bool SQLiteIRecordSet::MoveNext(void)
//{
//	GS_ASSERT_RET_VAL(m_pStmt, false);
//	return true;
//}

unsigned int SQLiteIRecordSet::GetColumns(void)
{
	GS_ASSERT_RET_VAL(m_pStmt, 0);
	return sqlite3_column_count(m_pStmt);
}

const char* SQLiteIRecordSet::GetColumnName(unsigned int iColIndex)
{
	GS_ASSERT_RET_VAL(m_pStmt, false);
	return sqlite3_column_name(m_pStmt, iColIndex);
}

bool SQLiteIRecordSet::GetValue(const char* szFieldName, 
			  void* pBuffer,
			  unsigned int iBufSize,
			  unsigned int* iFactLen,
			  EnumSQLLite3DataType eType)
{
	bool bRetValue = true;

	GS_ASSERT_RET_VAL(m_pStmt, false);
	GS_ASSERT_RET_VAL(szFieldName, false);
	GS_ASSERT_RET_VAL(pBuffer, false);
	GS_ASSERT_RET_VAL(iFactLen, false);

	if (iBufSize == 0)
	{
		GSAssert(iBufSize > 0);
		return false;
	}

	if(m_mapFieldList.empty())
	{
		GetFieldList();            //字段名与序号的map
	}

	unsigned int iIndex(0);        //获取字段在该行记录中的下标

	if(!GetFieldIndex(szFieldName, iIndex))      //szFieldName是否为表的一个字段
	{
		gs_printf("[SQLiteIRecordSet::GetValue] FieldName Error.\n");
		return false;
	}

	if (!DataTypeMatch(iIndex, eType))
	{
		gs_printf("[SQLiteIRecordSet::GetValue] DataType Error.\n");
		return false;
	}

	unsigned int iValLen = 0;  // 值长度
	switch(eType)
	{
	case TYPE_INT32:
		{
			*(int*)pBuffer = sqlite3_column_int(m_pStmt, iIndex);
		}
		break;
	case TYPE_INT64:
		{
			*(sqlite3_int64*)pBuffer = sqlite3_column_int64(m_pStmt, iIndex);
		}
		break;
	case TYPE_DOUBLE:
		{
			*(double*)pBuffer = sqlite3_column_double(m_pStmt, iIndex);
		}
		break;
	case TYPE_TEXT:
		{
			char* pTemp =  (char*)sqlite3_column_text(m_pStmt, iIndex);
			*(int*)iFactLen = sqlite3_column_bytes(m_pStmt, iIndex);
			if (*iFactLen > iBufSize)
			{
				gs_printf("[SQLiteIRecordSet::GetValue] Buffer Size Error.\n");
				return false;
			}
			else
			{
				memcpy(pBuffer, pTemp, *iFactLen);
			}
		}
		break;
	case TYPE_TEXT16:
		{
			// todo
			char* pTemp =  (char*)sqlite3_column_text16(m_pStmt, iIndex);
			*(int*)iFactLen = sqlite3_column_bytes16(m_pStmt, iIndex);
			if (*iFactLen > iBufSize)
			{
				gs_printf("[SQLiteIRecordSet::GetValue] Buffer Size Error.\n");
				return false;
			}
			else
			{
				memcpy(pBuffer, pTemp, *iFactLen);
			}
		}
		break;
	case TYPE_BLOB:
		{
			char* pTemp =  (char*)sqlite3_column_blob(m_pStmt, iIndex);
			*(int*)iFactLen = sqlite3_column_bytes(m_pStmt, iIndex);
			if (*iFactLen > iBufSize)
			{
				gs_printf("[SQLiteIRecordSet::GetValue] Buffer Size Error.\n");
				return false;
			}
			else
			{
				memcpy(pBuffer, pTemp, *iFactLen);
			}
		}
		break;
	default:
		GSAssert("[SQLiteIRecordSet::GetValue] data type invalid.");
		bRetValue = false;
	}
	return bRetValue;
}


bool SQLiteIRecordSet::BindField( unsigned int iRowIndex, 
							   EnumSQLLite3DataType eType, 
							   void *pBuffer, 
							   unsigned int iBufSize, 
							   bool bNull)
{
	int nCode = SQLITE_OK;
	GS_ASSERT_RET_VAL(m_pStmt, false);
	GS_ASSERT_RET_VAL(m_pSqlite, false);
	switch(eType)
	{
	case TYPE_INT32:
		{
			int iBuffer = *(int*)pBuffer;
			nCode = sqlite3_bind_int(m_pStmt, iRowIndex, iBuffer);
		}
		break;
	case TYPE_INT64:
		{
			sqlite_int64 iBuffer = *(sqlite_int64*)pBuffer;
			nCode = sqlite3_bind_int64(m_pStmt, iRowIndex, iBuffer);
		}
		break;
	case TYPE_DOUBLE:
		{
			double fBuffer = *(double*)pBuffer;
			nCode = sqlite3_bind_double(m_pStmt, iRowIndex, fBuffer);
		}
		break;
	case TYPE_TEXT:
		{
			if (bNull)
			{
				nCode = sqlite3_bind_null(m_pStmt, iRowIndex);
			}
			else
			{
				//char szBuffer[256] = {0};
				char* szBuffer = new char[2 * (iBufSize + 1)];
				if (szBuffer != NULL) {
					memset(szBuffer, 0, 2 * (iBufSize + 1));
					StrCodeUtil::GB2312ToUTF8(szBuffer, 2 * (iBufSize + 1), (const char*)pBuffer, iBufSize);
					std::string strBuffer(szBuffer);
					nCode = sqlite3_bind_text(m_pStmt, iRowIndex, (char*)strBuffer.c_str(), strBuffer.length(), SQLITE_TRANSIENT); // 需要拷贝一份
					delete [] szBuffer;
				}
			}
		}
		break;
	case TYPE_TEXT16:
		{
			// todo
		}
		break;
	case TYPE_BLOB:
		{
			if (bNull)
			{
				nCode = sqlite3_bind_zeroblob(m_pStmt, iRowIndex, iBufSize);
			}
			else
			{
				nCode = sqlite3_bind_blob(m_pStmt, iRowIndex, pBuffer, iBufSize, SQLITE_STATIC);
			}
		}
		break;
	default:
		GSAssert("[SQLiteIRecordSet::BindField] data type invalid.");
		return false;
	}
	if (nCode != SQLITE_OK)
	{
		return false;
	}
	return true;
}

bool SQLiteIRecordSet::ExecuteBind(void)
{
	if (sqlite3_step(m_pStmt) != SQLITE_DONE)
	{
		return false;
	}
	return true;
}

void SQLiteIRecordSet::ResetBindParam(void)
{
	// 重新初始化该sqlite3_stmt对象绑定的变量
	sqlite3_reset(m_pStmt);
}


bool SQLiteIRecordSet::DataTypeMatch(unsigned int iColumn, EnumSQLLite3DataType eType)
{
	bool bRetCode = false;
	GS_ASSERT_RET_VAL(m_pStmt, TYPE_DATA_UNKNOWN);

	//const char* pType = sqlite3_column_decltype(m_pStmt,iColumn);
	//if (pType)
	//{
	//	std::string strType = pType;
	//	if (strType.find("INT") != std::string::npos && (eType == TYPE_INT32 || eType == TYPE_INT64))
	//	{
	//		bRetCode = true;
	//	}
	//	else if ((strType.find("CHAR") != std::string::npos || strType.find("TEXT") != std::string::npos) 
	//		&& (eType == TYPE_TEXT || eType == TYPE_TEXT16))
	//	{
	//		bRetCode = true;
	//	}
	//	else if ((strType.find("REAL") != std::string::npos || strType.find("FLOA") != std::string::npos 
	//		|| strType.find("DOUB") != std::string::npos) && (eType == TYPE_DOUBLE))
	//	{
	//		bRetCode = true;
	//	}
	//	else if ((strType.find("BLOB") != std::string::npos) && (eType == TYPE_BLOB))
	//	{
	//		bRetCode = true;
	//	}
	//}
	
	 int iType = sqlite3_column_type(m_pStmt, iColumn);
	 switch (iType)
	 {
	 case SQLITE_INTEGER:
		 {
			 if (eType == TYPE_INT32 || eType == TYPE_INT64)
			 {
				bRetCode = true;
			 }
		 }
		 break;
	 case SQLITE_FLOAT:
		 {
			 if (eType == TYPE_DOUBLE)
			 {
				 bRetCode = true;
			 }
		 }
		 break;
	 case SQLITE3_TEXT:
		 {
			 if (eType == TYPE_TEXT || eType == TYPE_TEXT16)
			 {
				 bRetCode = true;
			 }
		 }
		 break;
	 case SQLITE_BLOB:
		 {
			 if (eType == TYPE_BLOB)
			 {
				 bRetCode = true;
			 }
		 }
		 break;
	 case SQLITE_NULL:
		 {
			 bRetCode = true;
			 break;
		 }
	 }

	 return bRetCode;
}

bool SQLiteIRecordSet::GetFieldList(void)
{
	GS_ASSERT_RET_VAL(m_pStmt, false);

	m_mapFieldList.clear();

	unsigned int iFieldNum =  sqlite3_column_count(m_pStmt);  // 字段数目

	for(unsigned int i = 0;i < iFieldNum;i ++)
	{
		const char* pFieldName = sqlite3_column_name(m_pStmt, i);
		if(!pFieldName)
			return false;

		// 转化为大写字符串
		std::string strUpperName = GSStrUtil::ToUpper(std::string(pFieldName));

		m_mapFieldList.insert(std::map<std::string, unsigned int>::value_type(strUpperName,i));
	}

	return true;
}

bool SQLiteIRecordSet::GetFieldIndex(const char* szFieldName,unsigned int& iIndex)
{
	GS_ASSERT_RET_VAL(szFieldName,false);

	std::string strNameUpper = GSStrUtil::ToUpper(std::string(szFieldName)); //转化为大写字符串

	std::map<std::string,  unsigned int>::iterator iter = m_mapFieldList.find(strNameUpper);

	if( iter == m_mapFieldList.end() )
		return false;

	iIndex = (*iter).second;

	return  true;
}



bool SQLiteIConnection::Open(const std::string& strFileName)
{
	GS_ASSERT_RET_VAL(!m_pSqlite, false);


#ifdef _WIN32
	// 把文件名转换为UTF8,sqlite3_open 只接受UTF8 文件名, 否则在有中文时出错误
	//std::string strUTF8FileName = AnsiToUTF8(strFileName.c_str());

	//char* pBuffer = new char[strFileName.size() + 1];
	//memset(pBuffer, 0, strFileName.size() + 1);
	//StrCodeUtil::GB2312ToUTF8(pBuffer, strFileName.size() + 1, strFileName.c_str(), strFileName.size());
	//std::string strUTF8FileName(pBuffer);
	//delete[] pBuffer;

	char szBuffer[256] = {0};
	StrCodeUtil::GB2312ToUTF8(szBuffer, sizeof(szBuffer), strFileName.c_str(), strFileName.size());
	std::string strUTF8FileName(szBuffer);

	/*if (sqlite3_open_v2(strUTF8FileName.c_str(), &m_pSqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL) != SQLITE_OK)
		return false;*/

	if (sqlite3_open(strUTF8FileName.c_str() , &m_pSqlite) != SQLITE_OK)
		return false;
#else
	if (sqlite3_open(strFileName.c_str() , &m_pSqlite) != SQLITE_OK)
		return false;
#endif	


	//	sqlite3_exec(m_pcsSqlite, "PRAGMA cache_size=8192;", NULL, NULL, NULL );	
	sqlite3_exec(m_pSqlite, "PRAGMA locking_mode=EXCLUSIVE;", NULL, NULL, NULL );
	sqlite3_exec(m_pSqlite, "PRAGMA synchronous=OFF;", NULL, NULL, NULL );			// synchronous=FULL NORMAL OFF  sqllite3默认Full，安全但速度慢
	sqlite3_exec(m_pSqlite, "PRAGMA journal_mode=PERSIST;",NULL,NULL,NULL);         // 日志文件在第一个transaction开始时创建，但是不被删除，直到exclusive access mode is exited

	//  sqlite3_exec(m_pSqlite, "PRAGMA vdbe_trace=false;", NULL, NULL, NULL );
    //sqlite3_exec(m_pSqlite, "PRAGMA fullfsync=false;", NULL, NULL, NULL );

	sqlite3_exec(m_pSqlite, "PRAGMA foreign_keys=true;", NULL, NULL, NULL );        
	sqlite3_exec(m_pSqlite, "PRAGMA temp_store=memory;", NULL, NULL, NULL );        // 在内存建立临时表

	return true;
}

bool SQLiteIConnection::IsOpen(void)
{
	return (m_pSqlite != NULL);
}


void SQLiteIConnection::Close(void)
{
	GS_ASSERT_RET(m_pSqlite);

	sqlite3_close(m_pSqlite);
	m_pSqlite = NULL;
}


bool SQLiteIConnection::ExecuteSql(const std::string & strSql)
{
	//char *pErrMsg = NULL;

	Int32 iRet = sqlite3_exec(m_pSqlite, strSql.c_str(), NULL, NULL, NULL/*  &pErrMsg*/);

	if( iRet != SQLITE_OK ) 
	{
		//::sqlite3_free(pErrMsg);	
		return false;
	}

	return true; 
}

Int64 SQLiteIConnection::ExecuteSqlRetID(const std::string & strSql)
{	
	//char *pErrMsg = NULL;

	Int32 iRet = sqlite3_exec(m_pSqlite, strSql.c_str(), NULL, NULL, NULL/*&pErrMsg*/);

	if( iRet != SQLITE_OK ) 
	{
		//::sqlite3_free(pErrMsg);	
		return -1;
	}

	return sqlite3_last_insert_rowid(m_pSqlite);
}


bool SQLiteIConnection::ExecuteQuery(const std::string & strSql, STRINGLIST & strResultList, int & iRows, int & iCols)
{
	char **ppResult = NULL;
	char *pErrMsg = NULL;

	int iRet = sqlite3_get_table(m_pSqlite, strSql.c_str(), &ppResult, &iRows, &iCols,  &pErrMsg);

	if(iRet != SQLITE_OK) 
	{
		::sqlite3_free(pErrMsg);	
		return false;
	}
	else
	{
		std::string strValues("");
		for (int i = 0; i < iRows; i ++)
		{
			for (int j = 0; j < iCols; j ++)
			{
				if (NULL != ppResult[(i + 1) * iCols + j]) {
					strValues = ppResult[(i + 1) * iCols + j];
				} else {
					strValues.clear();
				}
				strResultList.push_back(strValues);
			}
		}
	}

	sqlite3_free_table(ppResult);

	return true;
}

SQLiteIRecordSet* SQLiteIConnection::ExecutePageQuery(const std::string & strSql, int iStartRow, int iRowNum)
{
	std::string strFullSql;
	GSStrUtil::Format(strFullSql, "%s LIMIT %d,%d", strSql.c_str(), iStartRow, iRowNum);

	return PrepareRecordSet(strFullSql);
}


SQLiteIRecordSet* SQLiteIConnection::PrepareRecordSet(const std::string & strSql)
{
	SQLiteIRecordSet* pIRecordSet = NULL;
	sqlite3_stmt* pStmt3 = NULL;
	if (sqlite3_prepare_v2(m_pSqlite, strSql.c_str(), strSql.length(), &pStmt3, NULL) == SQLITE_OK) 
	{
		pIRecordSet = new SQLiteIRecordSet(m_pSqlite, pStmt3);
	}
	
	return pIRecordSet;
}



void SQLiteIConnection::ReleaseRecordSet(SQLiteIRecordSet* pIRecordSet)
{
	GS_ASSERT_RET(pIRecordSet);
	if (pIRecordSet)
	{
		delete pIRecordSet;
	}
}


bool SQLiteIConnection::BeginTrans(void)
{
	std::string strSQL = "BEGIN TRANSACTION";    
	return ExecuteSql(strSQL);
}

bool SQLiteIConnection::Rollback(void)
{
	std::string strSQL = "ROLLBACK TRANSACTION";
	return ExecuteSql(strSQL);
}

bool SQLiteIConnection::Commit(void)
{
	std::string strSQL = "COMMIT TRANSACTION";
	return ExecuteSql(strSQL);
}

const char* SQLiteIConnection::GetLastErrMsg(void)
{
	const char *pErr = sqlite3_errmsg(m_pSqlite);
	if ( !pErr )
		return "no error.";

	return  pErr;
}


int SQLiteIConnection::GetLastErrCode(void)
{
	return sqlite3_errcode(m_pSqlite);
}


sqlite3* SQLiteIConnection::GetSqlitePtr()
{
	return m_pSqlite;
}
