#include "GSFile.h"

GSFile::GSFile( void )
	: m_pFile(NULL)
{
}

GSFile::GSFile( const GSString& name )
	: m_strFullName(name),
	m_pFile(NULL)
{
}

GSFile::~GSFile(void)
{
	Close();
}

bool GSFile::Open( const GSString& mode )
{
	if (OS_SUCCESS != GSIsFile(m_strFullName.c_str())) {
		return false;
	}
	return Open(m_strFullName, mode);
}

bool GSFile::Open( const GSString& name, const GSString& mode )
{
	GS_ASSERT_RET_VAL(!IsOpen(), false);

	m_strFullName = name;
	if (!InnerOpen(m_strFullName, mode)) {
		return false;
	}
	return true;
}

void GSFile::Close( void )
{
	//static Int32 nCloseCount = 0;
	if (IsOpen()) {
		gsfclose(m_pFile);
		m_pFile = NULL;
		//++ nCloseCount;
		//gs_printf("Close File %d.\n", nCloseCount);
	}
}

GSString GSFile::GetFileName( void )
{
	const GSString strDelimiter("/");
	size_t nLastDelimiterPos = m_strFullName.find_last_of("/\\");
	if (GSString::npos == nLastDelimiterPos) {
		return GSString();
	}
	if (nLastDelimiterPos + 1 >= m_strFullName.size()) {
		return GSString();
	}
	return m_strFullName.substr(nLastDelimiterPos + 1);
}

#define MAX_FILE_BUFFER (1024)

bool GSFile::ReadAll( GSString& data )
{
	GS_ASSERT_RET_VAL(IsOpen(), false);

//#define USE_C_STL
#ifdef USE_C_STL
	if (gsfseek(m_pFile, 0, SEEK_END) != 0) {
		return false;
	}
	Int64 file_len = gsftell(m_pFile);
	if (gsfseek(m_pFile, 0, SEEK_SET) != 0) {
		return false;
	}
#else
	Int64 file_len = GSGetFileSize(m_strFullName.c_str());
#endif // USE_C_STL

	gsfseek(m_pFile, 0, SEEK_SET); // 从头开始
	UInt8 buffer[MAX_FILE_BUFFER] = {0};
	size_t byte_read = 0;
	while ((byte_read = gsfread(buffer, 1, MAX_FILE_BUFFER, m_pFile))) {
		data.append((const char*)buffer, byte_read);
	}

	// 需要测试两者性能
	//while (!gsfeof(m_pFile)) {
	//	gsfread(buffer, MAX_FILE_BUFFER, 1, m_pFile);
	//	if (MAX_FILE_BUFFER > file_len) {
	//		data.append(buffer, file_len);
	//	} else {
	//		data.append(buffer, MAX_FILE_BUFFER);
	//		file_len -= MAX_FILE_BUFFER;
	//	}
	//}

	if (!gsfeof(m_pFile)) {
		//if (gsferror(m_pFile)) {
		//	return false;
		//}
		return false;
	}

	return true;
}

bool GSFile::Write( const GSString& data )
{
	return Write(data.c_str(), data.size());
}

bool GSFile::Write( const char* data, size_t len )
{
	GS_ASSERT_RET_VAL(data && IsOpen(), false);

	size_t byte_write = gsfwrite(data, 1, len, m_pFile);
	if (byte_write == 0) { // 调用不成功
		return false;
	}
	if (byte_write != len) { // 没有写完
		return false;
	}

	return true;
}

bool GSFile::IsOpen( void )
{
	return (m_pFile != NULL);
}

GSString GSFile::GetFullName( void )
{
	return m_strFullName;
}

void GSFile::setFullName( const GSString& name )
{
	m_strFullName = name;
}

gsfile* GSFile::GetFileHandle( void )
{
	return m_pFile;
}

bool GSFile::InnerOpen( const GSString& name, const GSString& mode )
{
	m_pFile = gsfopen(name.c_str(), mode.c_str());
	if (m_pFile == NULL) {
		return false;
	}
	return true;
}
