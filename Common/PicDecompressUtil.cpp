#include "PicDecompressUtil.h"

#define MAX_PICTURE_SIZE		(1 * 1024 * 1024)

SystemInfo CPicDecompressUtil::m_system;

CPicDecompressUtil::CPicDecompressUtil(EnumGSPixFormat targetPixFormat /* = GS_PIX_NONE */)
	: m_pPicDecMemPool(NULL), m_targetPixFormat(targetPixFormat),
	m_decodePerfStat("GSDecodeStat")
{
	m_pPicDecMemPool = new GSMemoryPool(MAX_PICTURE_SIZE);
	GSAbort(m_pPicDecMemPool);
}

CPicDecompressUtil::~CPicDecompressUtil(void)
{
	if (m_pPicDecMemPool) {
		delete m_pPicDecMemPool;
		m_pPicDecMemPool = NULL;
	}
}

EnumGSPictureType CPicDecompressUtil::GetPicTypeFromSuffix( const GSString& strPicNameWithSuffix )
{
	size_t nFileSuffixPos = strPicNameWithSuffix.rfind('.');
	if (nFileSuffixPos == GSString::npos) {
		return GS_PIC_NONE;
	}
	if (nFileSuffixPos + 1 >= strPicNameWithSuffix.size()) {
		return GS_PIC_NONE;
	}

	GSString strFileSuffix = strPicNameWithSuffix.substr(nFileSuffixPos + 1);
	strFileSuffix = GSStrUtil::ToLower(strFileSuffix);

	return GSPicGetPictureType(strFileSuffix.c_str());
}

void* CPicDecompressUtil::AllocPicMemCB( int nsize, void *userdata )
{
	GS_ASSERT_RET_VAL(userdata, NULL);
	CPicDecompressUtil* pThis = (CPicDecompressUtil*) userdata;
	GS_ASSERT_RET_VAL(pThis->m_pPicDecMemPool, NULL);

	void* pBuffer = pThis->m_pPicDecMemPool->MallocEx(nsize);

	return pBuffer;
}

void CPicDecompressUtil::InitTargetPixFormat( EnumGSPixFormat targetPixFormat )
{
	m_targetPixFormat = targetPixFormat;
}

Int32 CPicDecompressUtil::DecodePicture( EnumGSPictureType ePicType, const void* pBuffer, Int32 nSize, void** pDecodeBuffer, Int32* pDecodeSize, Int32* nOutWidth, Int32* nOutHeight )
{
	GSPicDecompressor pDecompressor = m_libPicDecHandleMgr.GetHandle(ePicType);
	if (!pDecompressor) {
		Int32 nRetCode = GSPicDecompressorCreate(ePicType, 0, 0, m_targetPixFormat, &pDecompressor);
		if (nRetCode != PIC_SUCCESS) {
			UTIL_LOG("GSPicDecompressorCreate Fail. errCode:" << nRetCode, LOGGER_LEVEL_ERROR);
			return -1;
		}
	}

	//
	EnumGSPixFormat eOutpixType = GS_PIX_NONE;
	void* pOutBuffer = NULL;
	m_decodePerfStat.BeginAStat();
	Int32 nDecodeSize = GSDecompressMem2MemEx(pDecompressor, pBuffer, nSize, AllocPicMemCB, this, &pOutBuffer, nOutWidth, nOutHeight, &eOutpixType);
	m_libPicDecHandleMgr.SetHandle(ePicType, pDecompressor);
	if (nDecodeSize <= 0) {
		m_decodePerfStat.AddAFailStat();
		UTIL_LOG("GSDecompressMem2MemEx Fail. errCode:" << nDecodeSize, LOGGER_LEVEL_ERROR);
		if (pOutBuffer) {
			FreeDecodeMem(pOutBuffer);
			return -2;
		}
	} else {
		*pDecodeBuffer = pOutBuffer;
		*pDecodeSize = nDecodeSize;
	}
	m_decodePerfStat.EndAStat();

	return 0;
}

void CPicDecompressUtil::FreeDecodeMem( void* pData )
{
	GS_ASSERT_RET(pData);
	GS_ASSERT_RET(m_pPicDecMemPool);
	m_pPicDecMemPool->Free(pData);
}

void CPicDecompressUtil::InitForLog( SystemInfo* pSys )
{
	GS_ASSERT_RET(pSys);
	m_system = *pSys;
}

SystemInfo* CPicDecompressUtil::GetSystemInfo()
{
	return &m_system;
}
