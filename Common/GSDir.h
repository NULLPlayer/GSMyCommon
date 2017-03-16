#ifndef GS_DIR_H
#define GS_DIR_H

#include "CommonDef.h"

class GSDir
{
public:
	GSDir(void);
	GSDir(const GSString& strPath);
	~GSDir(void);

	GSString absolutePath(void) const;
	bool cd(const GSString& dirName);
	bool cdUp(void);
	GSString dirName(void) const;
	bool exists(const GSString& name);
	bool exists(void);
	bool mkpath(const GSString& dirPath) const;

private:
	GSString m_strAbsolutePath;
	GSString m_strExistName;
};

#endif // GS_DIR_H