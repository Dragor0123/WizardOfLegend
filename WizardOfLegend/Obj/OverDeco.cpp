#include "../stdafx.h"
#include "OverDeco.h"
#include "../MyBitmap/BmpMgr.h"
#include "../MyBitmap/MyBitmap.h"

COverDeco::COverDeco()
	:m_pMyBitmap(nullptr)
{
}


COverDeco::~COverDeco()
{
}

bool COverDeco::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/DecoEffect/PlazaArch.bmp", "PlazaArch"))
		return false;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/DecoEffect/PurpleTent.bmp", "PurpleTent"))
		return false;

	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/DecoEffect/Easel.bmp", "Easel"))
		return false;

	m_pMyBitmap = CBmpMgr::Get_Instance()->Find_MyBitmap(m_strFrameKey);
	if (!m_pMyBitmap)
		return false;

	BITMAP bitmap;
	HBITMAP hBitmap = m_pMyBitmap->Get_Bitmap();
	if (!hBitmap)
		return false;

	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bitmap);

	m_tInfo.iCX = (int)bitmap.bmWidth;
	m_tInfo.iCY = (int)bitmap.bmHeight;

	Update_Rect();
	return true;
}

int COverDeco::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void COverDeco::Late_Update(float _fdTime)
{
}

void COverDeco::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (m_pMyBitmap)
	{
		HDC hMemDC = m_pMyBitmap->Get_MemDC();
		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY)
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, MAGENTA_COLOR);
	}

	// Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void COverDeco::Release()
{
}

void COverDeco::Key_Check(float _fdTime)
{
}
