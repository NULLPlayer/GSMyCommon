#include "GSDir.h"
#include "GSSysApi.h"

GSDir::GSDir(void)
{
	char szPath[MAX_PATH] = {'\0'};
	GSGetCurrentDir(szPath, MAX_PATH);
	m_strAbsolutePath = szPath;
}

GSDir::GSDir( const GSString& strPath )
{
	m_strAbsolutePath = strPath;
}

GSDir::~GSDir(void)
{
}

GSString GSDir::absolutePath() const
{
	return m_strAbsolutePath;
}

bool GSDir::cd( const GSString& dirName )
{
#if defined(_WIN32) || defined(_WIN64)
	const GSString strDelimiter("\\");
#endif
	m_strAbsolutePath += strDelimiter + dirName;
	return (OS_SUCCESS == GSIsFolder(m_strAbsolutePath.c_str()));
}

bool GSDir::cdUp( void )
{
	GSString strTmp = m_strAbsolutePath;
	size_t nLastDelimiterPos = strTmp.find_last_of("/\\");
	if (GSString::npos == nLastDelimiterPos) {
		return false;
	}
	m_strAbsolutePath = strTmp.substr(0, nLastDelimiterPos);
	return true;
}

GSString GSDir::dirName( void ) const
{
	GSString strTmp = m_strAbsolutePath;
	size_t nLastDelimiterPos = strTmp.find_last_of("/\\");
	if (GSString::npos == nLastDelimiterPos) {
		return GSString();
	}
	if (nLastDelimiterPos + 1 >= strTmp.size()) {
		return GSString();
	}
	return strTmp.substr(nLastDelimiterPos + 1);
}

bool GSDir::exists( const GSString& name )
{
	if (GSIsFolderEmpty(m_strAbsolutePath.c_str())) {
		return false;
	}

#if defined(_WIN32) || defined(_WIN64)
	const GSString strDelimiter("\\");
#endif

	GSString strFullName = m_strAbsolutePath + strDelimiter + name;
	EnumGSOSErrno eIsFile = GSIsFileExist(strFullName.c_str());
	if (eIsFile == OS_SUCCESS) {
		return true;
	}

	EnumGSOSErrno eIsFold = GSIsFolder(strFullName.c_str());
	if (eIsFold == OS_SUCCESS) {
		return true;
	}

	return false;
}

bool GSDir::exists( void )
{
	return exists(m_strAbsolutePath);
}

bool GSDir::mkpath( const GSString& dirPath ) const
{
	if (OS_SUCCESS != GSRecursiveMakeDir(dirPath.c_str())) {
		return false;
	}
	return true;
}
