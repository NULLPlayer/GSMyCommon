#ifndef GS_FILE_H
#define GS_FILE_H

#include <string>
#include "GSDefs.h"
#include "GSTypes.h"
#include "GSStrUtil.hpp"
#include "GSSysApi.h"

class GSFile
{
public:
	typedef enum _EnumFileDataType {
		E_TEXT,
		E_BINARY,
	} EnumFileDataType;

public:
	GSFile(void);
	GSFile(const GSString& name);
	~GSFile(void);

	bool Open(const GSString& mode);
	bool Open(const GSString& name, const GSString& mode);
	void Close(void);

	GSString GetFileName(void);

	bool ReadAll(GSString& data);
	bool Write(const GSString& data);
	bool Write(const char* data, size_t len);

	bool IsOpen(void);

	GSString GetFullName(void);
	void setFullName(const GSString& name);

	gsfile* GetFileHandle(void);

private:
	bool InnerOpen(const GSString& name, const GSString& mode);

private:
	GSString m_strFullName;
	//GSString m_strMode;
	gsfile* m_pFile;
};

/**
	r 打开只读文件，该文件必须存在。
　　r+ 打开可读写的文件，该文件必须存在。
　　rb+ 读写打开一个二进制文件，只允许读写数据。
　　rt+ 读写打开一个文本文件，允许读和写。
　　w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
　　w+ 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
　　a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
　　a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）
　　wb 只写打开或新建一个二进制文件；只允许写数据。
　　wb+ 读写打开或建立一个二进制文件，允许读和写。
　　wt+ 读写打开或着建立一个文本文件；允许读写。
　　at+ 读写打开一个文本文件，允许读或在文本末追加数据。
　　ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
**/

#endif // GS_FILE_H