#ifndef GS_WIN_FILE_H
#define GS_WIN_FILE_H

#include <WinError.h>
#include "CommonDef.h"

/**
|                    When the file...
This argument:           |             Exists            Does not exist
-------------------------+------------------------------------------------------
CREATE_ALWAYS            |            Truncates             Creates
CREATE_NEW         +-----------+        Fails               Creates
OPEN_ALWAYS     ===| does this |===>    Opens               Creates
OPEN_EXISTING      +-----------+        Opens                Fails
TRUNCATE_EXISTING        |            Truncates              Fails
*/

class GSWinFile
{
public:
	GSWinFile(void);
	GSWinFile(const GSString& name);
	~GSWinFile(void);

public:
	DWORD Open(DWORD dwDesiredAccess, 
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile);
	DWORD Open(LPCSTR lpFileName, 
		DWORD dwDesiredAccess, 
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile);
	VOID Close();

	BOOL ReadAll(GSString& data);
	BOOL Write(const GSString& data);

	BOOL IsOpen();

private:
	GSString m_strFullName;
	HANDLE m_pFile;
};

#endif // GS_WIN_FILE_H