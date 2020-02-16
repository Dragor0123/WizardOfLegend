#pragma once

#ifndef __BMPMGR_H__
#define __BMPMGR_H__

#include "Singleton.h"

class CMyBitmap;

class CBmpMgr : public Singleton<CBmpMgr>
{
	friend class Singleton<CBmpMgr>;

public:
	bool Initialize(HINSTANCE _hInst, HDC _DC);
	CMyBitmap* Load_Bitmap(const string& strKey,
		const wchar_t* pFileName, const string& strPathKey = BITMAP_PATH);
	CMyBitmap* Find_Bitmap(const string& strKey);
	CMyBitmap* Get_BackBuffer() const;
	void Release();

private:
	CBmpMgr();
	virtual ~CBmpMgr();
	CBmpMgr(const CBmpMgr&) = delete;
	CBmpMgr& operator=(const CBmpMgr&) = delete;

private:
	unordered_map<string, CMyBitmap*> m_mapStrBmp;
	HINSTANCE		m_hInst;
	HDC				m_hDC;
	CMyBitmap*		m_pBackBuffer;
};

#endif

