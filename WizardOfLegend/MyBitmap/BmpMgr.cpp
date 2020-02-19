#include "../stdafx.h"
#include "BmpMgr.h"
#include "MyBitmap.h"

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

bool CBmpMgr::Initialize()
{
	if (!Insert_Bmp(L"Bitmap/BackBuffer.bmp", "BackBuffer"))
		return false;
	return true;
}

bool CBmpMgr::Insert_Bmp(const TCHAR * _pFilePath, const string & _strImageKey)
{
	auto iter = m_mapStrBmp.find(_strImageKey);

	if (m_mapStrBmp.end() == iter)
	{
		CMyBitmap* pBmp = new CMyBitmap;
		if (!pBmp->Load_Bmp(_pFilePath)) {
			SAFE_DELETE(pBmp);
			MessageBox(NULL, L"파일 없음", L"파일 없음", MB_OK);
			return false;
		}

		m_mapStrBmp.emplace(_strImageKey, pBmp);
	}
	return true;
}

CMyBitmap * CBmpMgr::Find_MyBitmap(const string & _strImageKey)
{
	auto iter = m_mapStrBmp.find(_strImageKey);
	if (m_mapStrBmp.end() == iter)
		return NULL;

	return iter->second;
}

HDC CBmpMgr::Find_Image(const string & _strImageKey)
{
	auto iter = m_mapStrBmp.find(_strImageKey);
	if (m_mapStrBmp.end() == iter)
		return NULL;

	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	Safe_Delete_Map(m_mapStrBmp);
}
