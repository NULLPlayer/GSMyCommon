#ifndef _LIBPIC_HANDLE_MANAGE_H_
#define _LIBPIC_HANDLE_MANAGE_H_

#include <list>
#include <map>
#include "libPicture.h"
#include "GSMutex.h"

typedef std::list<GSPicDecompressor>  PicDecompressorList;
typedef PicDecompressorList::iterator PicDecompressorListIt;

typedef std::map<Int32, PicDecompressorList> PicDecompressorMap;
typedef PicDecompressorMap::iterator         PicDecompressorMapIt;
typedef PicDecompressorMap::value_type       PicDecompressorMapValue;

class CLibPicDecHandleMgr
{
public:
	CLibPicDecHandleMgr(void) {}
	~CLibPicDecHandleMgr(void) { CleanupHandles(); }

public:
	// ÉèÖÃ½âÂë¾ä±ú
	void SetHandle(Int32 nType, GSPicDecompressor hPicDecompressor)
	{
		GS_ASSERT_RET(hPicDecompressor);

		GSAutoMutex csAuto(m_mutexLock);
		PicDecompressorMapIt it = m_mapPicDecompressor.find(nType);
		if (it == m_mapPicDecompressor.end())
		{
			PicDecompressorList listDecompressor;
			listDecompressor.push_back(hPicDecompressor);
			m_mapPicDecompressor[nType] = listDecompressor;
		}
		else
		{
			it->second.push_back(hPicDecompressor);
		}
	}

	// »ñÈ¡½âÂë¾ä±ú
	GSPicDecompressor GetHandle(Int32 nType)
	{
		GSPicDecompressor hPicDecompressor = NULL;
		GSAutoMutex csAuto(m_mutexLock);

		PicDecompressorMapIt it = m_mapPicDecompressor.find(nType);
		if (it == m_mapPicDecompressor.end())
		{
			return NULL;
		}
		else if (!it->second.empty())
		{
			hPicDecompressor = it->second.front();
			it->second.pop_front();
		}

		return hPicDecompressor;
	}

private:
	// ÊÍ·Å½âÂë¾ä±ú
	void CleanupHandles(void)
	{
		GSAutoMutex csAuto(m_mutexLock);
		for (PicDecompressorMapIt it = m_mapPicDecompressor.begin(); it != m_mapPicDecompressor.end(); it ++)
		{
			for (PicDecompressorListIt ir = it->second.begin(); ir != it->second.end(); ir ++)
			{
				GSPicDecompressor hPicDecompressor = *ir;
				if (hPicDecompressor)
				{
					GSPicDecompressorDestory(&hPicDecompressor);
				}
			}
		}

		m_mapPicDecompressor.clear();
	}

private:
	PicDecompressorMap  m_mapPicDecompressor;
	GSMutex			    m_mutexLock;
};



typedef std::list<GSPicCompressor>  PicCompressorList;
typedef PicCompressorList::iterator PicCompressorListIt;

typedef std::map<Int32, PicCompressorList> PicCompressorMap;
typedef PicCompressorMap::iterator         PicCompressorMapIt;
typedef PicCompressorMap::value_type       PicCompressorMapValue;

class CLibPicCompHandleMgr
{
public:
	CLibPicCompHandleMgr(void) {}
	~CLibPicCompHandleMgr(void) {}
public:
	// ÉèÖÃ½âÂë¾ä±ú
	void SetHandle(Int32 nType, GSPicCompressor hPicCompressor)
	{
		GS_ASSERT_RET(hPicCompressor);

		GSAutoMutex csAuto(m_mutexLock);
		PicCompressorMapIt it = m_mapPicCompressor.find(nType);
		if (it == m_mapPicCompressor.end())
		{
			PicCompressorList listCompressor;
			listCompressor.push_back(hPicCompressor);
			m_mapPicCompressor[nType] = listCompressor;
		}
		else
		{
			it->second.push_back(hPicCompressor);
		}
	}

	// »ñÈ¡½âÂë¾ä±ú
	GSPicCompressor GetHandle(Int32 nType)
	{
		GSPicCompressor hPicCompressor = NULL;
		GSAutoMutex csAuto(m_mutexLock);

		PicCompressorMapIt it = m_mapPicCompressor.find(nType);
		if (it == m_mapPicCompressor.end())
		{
			return NULL;
		}
		else if (!it->second.empty())
		{
			hPicCompressor = it->second.front();
			it->second.pop_front();
		}

		return hPicCompressor;
	}

	// ÊÍ·Å½âÂë¾ä±ú
	void CleanupHandles(void)
	{
		GSAutoMutex csAuto(m_mutexLock);
		for (PicCompressorMapIt it = m_mapPicCompressor.begin(); it != m_mapPicCompressor.end(); it ++)
		{
			for (PicCompressorListIt ir = it->second.begin(); ir != it->second.end(); ir ++)
			{
				GSPicCompressor hPicCompressor = *ir;
				if (hPicCompressor)
				{
					GSPicCompressorDestory(&hPicCompressor);
				}
			}
		}

		m_mapPicCompressor.clear();
	}

private:
	PicCompressorMap  m_mapPicCompressor;
	GSMutex			  m_mutexLock;
};

#endif // _LIBPIC_HANDLE_MANAGE_H_