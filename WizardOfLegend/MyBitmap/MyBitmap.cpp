#include "../stdafx.h"
#include "MyBitmap.h"


CMyBitmap::CMyBitmap()
	: m_hMemDC(NULL), m_hBitmap(NULL), m_hOldBmp(NULL)
{
}


CMyBitmap::~CMyBitmap()
{
	Release();
}

bool CMyBitmap::Load_Bmp(const TCHAR * _pFilePath)
{
	HDC hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL, _pFilePath, IMAGE_BITMAP, 0, 0
		,LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (NULL == m_hBitmap)
		return false;

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
}

void CMyBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
