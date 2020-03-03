#pragma once

#ifndef __MYBITMAP_H__
#define __MYBITMAP_H__

class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();

public:
	HDC Get_MemDC() { return m_hMemDC; }
	HBITMAP Get_Bitmap() const { return m_hBitmap; }

	bool Set_Bitmap(HBITMAP hNewBMP);
	bool Load_Bmp(const TCHAR* _pFilePath);
	void Release();

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;
};

#endif