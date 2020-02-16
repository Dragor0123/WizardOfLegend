#include "../stdafx.h"
#include "MyBitmap.h"
#include "../Manager/PathMgr.h"


bool CMyBitmap::Load_Bitmap(HINSTANCE hInst, HDC hDC, const string & strKey, const wchar_t * pFileName, const string & strPathKey)
{
	//�޸� DC�� ������ش�.
	m_hMemDC = CreateCompatibleDC(hDC);

	//��ü ��θ� ������ش�.
	const wchar_t* pPath = CPathMgr::Get_Instance()->Find_Path(strPathKey);
	wstring strPath;
	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (!m_hBitmap)
		return false;

	// ������ ������� ��Ʈ�� ������ dc�� �����Ѵ�.
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(m_tBmpInfo), &m_tBmpInfo);

	return true;
}

void CMyBitmap::Set_ColorKey(unsigned char r, unsigned char g, unsigned char b)
{
	m_ColorKey = RGB(r, g, b);
	m_bColorKeyEnable = true;
}

void CMyBitmap::Set_ColorKey(COLORREF colorKey)
{
	m_ColorKey = colorKey;
	m_bColorKeyEnable = true;
}

CMyBitmap::CMyBitmap()
	: m_hMemDC(NULL), m_hBitmap(NULL), m_hOldBitmap(NULL),
	m_bColorKeyEnable(false), m_ColorKey(RGB(255, 0, 255))
{
	ZeroMemory(&m_tBmpInfo, sizeof(BITMAP));
}

CMyBitmap::~CMyBitmap()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
