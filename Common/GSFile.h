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

	size_t Read(void* buf, size_t nByte);
	bool ReadAll(GSString& data);
	bool Write(const GSString& data);
	bool Write(const void* buf, size_t nByte);

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
	r ��ֻ���ļ������ļ�������ڡ�
����r+ �򿪿ɶ�д���ļ������ļ�������ڡ�
����rb+ ��д��һ���������ļ���ֻ�����д���ݡ�
����rt+ ��д��һ���ı��ļ����������д��
����w ��ֻд�ļ������ļ��������ļ�������Ϊ0�������ļ����ݻ���ʧ�����ļ��������������ļ���
����w+ �򿪿ɶ�д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������������ļ���
����a �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�����ļ�ԭ�ȵ����ݻᱻ��������EOF��������
����a+ �Ը��ӷ�ʽ�򿪿ɶ�д���ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������ ��ԭ����EOF����������
����wb ֻд�򿪻��½�һ���������ļ���ֻ����д���ݡ�
����wb+ ��д�򿪻���һ���������ļ����������д��
����wt+ ��д�򿪻��Ž���һ���ı��ļ��������д��
����at+ ��д��һ���ı��ļ�������������ı�ĩ׷�����ݡ�
����ab+ ��д��һ���������ļ�������������ļ�ĩ׷�����ݡ�
**/

#endif // GS_FILE_H