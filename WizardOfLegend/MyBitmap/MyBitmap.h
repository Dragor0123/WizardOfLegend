#pragma once

#ifndef __MYBITMAP_H__
#define __MYBITMAP_H__

#include "../Ref.h"

class CBmpMgr;

class CMyBitmap : public CRef
{
	friend class CBmpMgr;

public:
	bool Load_Bitmap(HINSTANCE hInst, HDC hDC, const string& strKey,
		const wchar_t* pFileName, const string& strPathKey = BITMAP_PATH);

	HDC Get_DC() const { return m_hMemDC; }

	void Set_ColorKey(unsigned char r, unsigned char g, unsigned char b);
	void Set_ColorKey(COLORREF colorKey);
	COLORREF Get_ColorKey() const { return m_ColorKey; }
	bool Get_ColorKeyEnable() const { return m_bColorKeyEnable; }

private:
	CMyBitmap();
	CMyBitmap(const CMyBitmap&) = delete;
	CMyBitmap& operator=(const CMyBitmap&) = delete;
	~CMyBitmap();

private:
	HDC				m_hMemDC;
	HBITMAP			m_hBitmap;
	HBITMAP			m_hOldBitmap;
	BITMAP			m_tBmpInfo;
	COLORREF		m_ColorKey;
	bool			m_bColorKeyEnable;
};

#endif