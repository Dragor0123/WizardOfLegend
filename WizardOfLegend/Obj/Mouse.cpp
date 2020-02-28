#include "../stdafx.h"
#include "Mouse.h"
#include "../Manager/ScrollMgr.h"
#include "../MyBitmap/BmpMgr.h"
CMouse::CMouse()
{
}

CMouse::~CMouse()
{
	Release();
}

bool CMouse::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/UI_MOUSE.bmp", "MouseIcon"))
		return false;

	m_strFrameKey = "MouseIcon";
	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;
	Equalize_HitSzInfoSz();

	return true;
}

void CMouse::Key_Check(float _fdTime)
{
	//
}

int CMouse::Update(float _fdTime)
{
	Key_Check(_fdTime);

	POINT	pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	ShowCursor(false);

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;

	Equalize_HitPosInfoPos();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CMouse::Late_Update(float _fdTime)
{
}

void CMouse::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("MouseIcon");

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left)
		, (int)(m_tRect.top)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);
}

void CMouse::Release()
{
}
