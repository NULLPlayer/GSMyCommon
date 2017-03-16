#include "LocalDBConnection.h"

CLocalAutoDBConnection::CLocalAutoDBConnection(const GSString& strFileName)
{
	m_pConnection = new SQLiteIConnection;
	if (m_pConnection) {
		if (! m_pConnection->Open(strFileName)) {
			gs_printf("Open Database File Fail. Database FilePath:%s\n", strFileName.c_str());
			delete m_pConnection;
			m_pConnection = NULL;
		}
	}
}

CLocalAutoDBConnection::~CLocalAutoDBConnection(void)
{
	if (m_pConnection != NULL) {
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}
}

SQLiteIConnection* CLocalAutoDBConnection::GetConnection()
{
	return m_pConnection;
}
