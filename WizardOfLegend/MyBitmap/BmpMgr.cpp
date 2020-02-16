#include "../stdafx.h"
#include "BmpMgr.h"
#include "MyBitmap.h"

bool CBmpMgr::Initialize(HINSTANCE _hInst, HDC _DC)
{
	m_hInst = _hInst;
	m_hDC = _DC;
	m_pBackBuffer = Load_Bitmap("Backbuffer", L"Backbuffer.bmp", BITMAP_PATH);

	return true;
}

CMyBitmap * CBmpMgr::Load_Bitmap(const string & strKey, const wchar_t * pFileName, const string & strPathKey)
{
	CMyBitmap* pBitmap = Find_Bitmap(strKey);
	if (pBitmap)
		return pBitmap;

	pBitmap = new CMyBitmap;

	if (!pBitmap->Load_Bitmap(m_hInst, m_hDC, strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pBitmap);
		return nullptr;
	}

	pBitmap->AddRef();
	m_mapStrBmp.emplace(strKey, pBitmap);

	return pBitmap;
}

CMyBitmap * CBmpMgr::Find_Bitmap(const string & strKey)
{
	auto iter = m_mapStrBmp.find(strKey);
	if (m_mapStrBmp.end() == iter)
		return nullptr;

	iter->second->AddRef();
	return iter->second;
}

CMyBitmap * CBmpMgr::Get_BackBuffer() const
{
	m_pBackBuffer->AddRef();
	return m_pBackBuffer;
}

HDC CBmpMgr::Get_Bitmap_DC(const string & strKey)
{
	CMyBitmap* pBitmap = Find_Bitmap(strKey);
	if (pBitmap)
		return pBitmap->Get_DC();
	else
		return NULL;
}

void CBmpMgr::Release()
{
	SAFE_RELEASE(m_pBackBuffer);
	Safe_Release_Map(m_mapStrBmp);
}

CBmpMgr::CBmpMgr()
	: m_hInst(NULL), m_hDC(NULL), m_pBackBuffer(nullptr)
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}
