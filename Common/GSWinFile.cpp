#include "GSWinFile.h"
#include <Windows.h>

GSWinFile::GSWinFile(void) 
	: m_pFile(INVALID_HANDLE_VALUE)
{
}

GSWinFile::GSWinFile( const GSString& name )
	: m_pFile(INVALID_HANDLE_VALUE)
{
	m_strFullName = name;
}

GSWinFile::~GSWinFile(void)
{
	Close();
}

DWORD GSWinFile::Open( DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	return Open(m_strFullName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

DWORD GSWinFile::Open( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	GS_ASSERT_RET_VAL(!IsOpen(), -1);

	SetLastError(ERROR_SUCCESS);
	m_pFile = CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	DWORD nRet = GetLastError();
	return nRet;
}

VOID GSWinFile::Close()
{
	if (IsOpen()) {
		CloseHandle(m_pFile);
		m_pFile = INVALID_HANDLE_VALUE;
	}
}

#define MAX_FILE_BUFFER (1024)

BOOL GSWinFile::ReadAll( GSString& data )
{
	GS_ASSERT_RET_VAL(IsOpen(), FALSE);

	//Int64 file_len = GSGetFileSize(m_strFullName.c_str());
	UInt8 buffer[MAX_FILE_BUFFER] = {0};

	DWORD dwReadSize = 0;
	SetLastError(ERROR_SUCCESS);
	while (ReadFile(m_pFile, buffer, MAX_FILE_BUFFER, &dwReadSize, NULL)) {
		if (dwReadSize == 0) { // eof
			break;
		}
		data.append((const char*)buffer, dwReadSize);
	}

	if (ERROR_SUCCESS != GetLastError()) {
		return FALSE;
	}

	return TRUE;
}

BOOL GSWinFile::Write( const GSString& data )
{
	GS_ASSERT_RET_VAL(IsOpen(), FALSE);

	DWORD dwWritenSize = 0;
	//OVERLAPPED overlapped;
	//memset(&overlapped, 0, sizeof(overlapped));
	SetLastError(ERROR_SUCCESS);
	BOOL bRet = WriteFile(m_pFile, data.c_str(), (DWORD)data.size(), &dwWritenSize, NULL);
	if (!bRet || dwWritenSize != data.size() || ERROR_SUCCESS != GetLastError()) {
		return FALSE;
	}
	return TRUE;
}

BOOL GSWinFile::IsOpen()
{
	return (m_pFile != INVALID_HANDLE_VALUE);
}
