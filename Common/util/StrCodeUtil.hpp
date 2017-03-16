#ifndef _STR_CODE_UTIL_H_
#define _STR_CODE_UTIL_H_

#include <iostream>

#ifdef	_LINUX

#include <errno.h>
#include <iconv.h>

#endif


class StrCodeUtil
{
#define MAX_URL_LEN  1024
public:
	StrCodeUtil(void) {}
	~StrCodeUtil(void) {}

	static int UTF8ToGB2312(char *pOutBuffer, int nMaxOutLen, const char *pInBuffer, int nInLen)
	{
#ifdef _WIN32

		int nSize = 0;
		int nLength = MultiByteToWideChar(CP_UTF8, 0, pInBuffer, -1, NULL, 0);
		wchar_t* pwStr = (wchar_t*)malloc((nLength + 1) * sizeof(wchar_t));
		if (pwStr)
		{
			MultiByteToWideChar(CP_UTF8, 0, pInBuffer, -1, pwStr, nInLen);
			nSize = WideCharToMultiByte(CP_ACP, 0, pwStr, -1, pOutBuffer, nMaxOutLen, NULL, NULL);
		    pOutBuffer[nSize] = '\0';

			free(pwStr);
		}

		return nSize;
#else

		char *pIn = (char *)pInBuffer;
		char *pOut = pOutBuffer;

		size_t nRet;
		size_t nLeftLen = nMaxOutLen;
		iconv_t cd;

		cd = iconv_open("GB2312", "UTF-8");
		if (cd == (iconv_t) - 1)
		{
			return -1;
		}
		size_t nSrcLen = nInLen;
		nRet = iconv(cd, &pIn,&nSrcLen, &pOut, &nLeftLen);
		if (nRet == - 1)
		{
			iconv_close(cd);
			return -1;
		}

		iconv_close(cd);

		return (nMaxOutLen - nLeftLen);

#endif
	}


	static int GB2312ToUTF8(char *pOutBuffer, int nMaxOutLen, const char *pInBuffer, int nInLen)
	{

#ifdef _WIN32

		int nSize = 0;
		int nLength = MultiByteToWideChar(CP_ACP, 0, pInBuffer, -1, NULL, 0);
		wchar_t* pwStr = (wchar_t*)malloc((nLength + 1) * sizeof(wchar_t));
		if (pwStr)
		{
			MultiByteToWideChar(CP_ACP, 0, pInBuffer, -1, pwStr, nLength);
			nSize = WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, pOutBuffer, nMaxOutLen, NULL, NULL);
			pOutBuffer[nSize] = '\0';

			free(pwStr);
		}

		return nSize;

#else

		char *pIn = (char *)pInBuffer;
		char *pOut = pOutBuffer;
		size_t nRet;
		size_t iLeftLen=nMaxOutLen;
		iconv_t cd;

		cd = iconv_open("UTF-8", "GB2312");
		if (cd == (iconv_t) - 1)
		{
			return -1;
		}
		size_t nSrcLen=nInLen;
		nRet = iconv(cd, &pIn, &nSrcLen, &pOut, &nLeftLen);
		if (nRet == - 1)
		{
			iconv_close(cd);
			return -1;
		}

		iconv_close(cd);

		return (nMaxOutLen - nLeftLen);

#endif
	}   

	static std::string UrlEncode(char *pUrl)
	{
		char szUrl[MAX_URL_LEN] = {0};
		size_t nLen = GB2312ToUTF8(szUrl, MAX_URL_LEN, pUrl, strlen(pUrl));

		std::string strURL;
		for (size_t i = 0; i < nLen; i++)
		{
			unsigned char ch = (unsigned char)szUrl[i];
			if (isascii(ch))
			{
				if (isspace((unsigned char)szUrl[i]))
				{
				//	strURL.append("+");
					strURL.append("%20");
				}
				else
				{
					strURL += ch;
				}
			}
			else
			{
				char tempBuf[4] = {0};
				sprintf(tempBuf,"%%%X%X", ch >> 4, ch % 16);
				strURL.append(tempBuf);
			}
		}
		return strURL;
	}

	


	static std::string UrlDecode(char *pUrl)
	{
		std::string strURL;
		char tmp[2] = {0};
		size_t i = 0, idx = 0, len = strlen(pUrl);

		while(i < len)
		{
			if(pUrl[i] == '%')
			{
				tmp[0] = pUrl[i+1];
				tmp[1] = pUrl[i+2];
				strURL += StrToBin(tmp);
				i = i + 3;
			}
			else if(pUrl[i] == '+')
			{
				strURL += ' ';
				i++;
			}
			else
			{
				strURL += pUrl[i];
				i++;
			}
		}

		char szUrl[MAX_URL_LEN] = {0};

		UTF8ToGB2312(szUrl, MAX_URL_LEN, strURL.c_str(), strURL.size());

		return szUrl;
	}


private:
	//½âUrlÊ¹ÓÃ
	static char CharToInt(char ch)
	{
		if(ch>='0' && ch<='9')return (char)(ch-'0');
		if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
		if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
		return -1;
	}

	static char StrToBin(char *pStr)
	{
		char tempWord[2];
		char chn;

		tempWord[0] = CharToInt(pStr[0]);                         //make the B to 11 -- 00001011
		tempWord[1] = CharToInt(pStr[1]);                         //make the 0 to 0  -- 00000000

		chn = (tempWord[0] << 4) | tempWord[1];					  //to change the BO to 10110000

		return chn;
	}
};

#endif