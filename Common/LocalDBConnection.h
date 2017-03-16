#ifndef _LOCAL_DB_CONNECTION_H_
#define _LOCAL_DB_CONNECTION_H_

#include "GSDefs.h"
#include "GSMutex.h"
#include "GSStrUtil.hpp"
#include "pluginInfo.h"
#include "Singleton.hpp"
#include "GSTime.h"
#include "sqlite3/SQLite3Wrapper.h"
#include <string>
#include <list>

using std::string;

class CLocalAutoDBConnection
{
public:
	CLocalAutoDBConnection(const GSString& strFileName);
	virtual ~CLocalAutoDBConnection(void);

public:
	SQLiteIConnection* GetConnection();

private:
	SQLiteIConnection* m_pConnection;
};

#endif
