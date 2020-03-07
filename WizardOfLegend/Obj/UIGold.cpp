#include "../stdafx.h"
#include "UIGold.h"
#include "../MyBitmap/BmpMgr.h"
#include "Player.h"

CUIGold::CUIGold()
{
}


CUIGold::~CUIGold()
{
	Release();
}

bool CUIGold::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/UI_MONEY.bmp", "UI_GOLD"))
		return false;
	
	if (!m_pTarget)
		return false;

	m_tInfo.iCX = 32;
	m_tInfo.iCY = 32;
	m_tInfo.fX = (float)(WINCX >> 1) - 10.f;
	m_tInfo.fY = (float)WINCY - 55.f;

	m_strFrameKey = "UI_GOLD";
	m_eRenderGroupID = GROUPID::UI;
	Update_Rect();
	return true;
}

int CUIGold::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	// 쿨타임 받아오는게 업데이터 할 거임!
	return OBJ_NOEVENT;
}

void CUIGold::Late_Update(float _fdTime)
{
}

void CUIGold::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left)
		, (int)(m_tRect.top)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	HFONT hFont, hOldFont;
	hFont = CreateFont(28, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("Consolas"));
	// Arial
	TCHAR szBuff[16] = L"";
	swprintf_s(szBuff, 16, L" %3d",
		static_cast<CPlayer*>(m_pTarget)->Get_Gold());
	hOldFont = (HFONT)SelectObject(_DC, hFont);
	COLORREF oldFontColor = SetTextColor(_DC, RGB(255, 255, 255));
	SetBkMode(_DC, TRANSPARENT);
	TextOut(_DC, (int)(m_tInfo.fX + 32), m_tRect.top, szBuff, lstrlen(szBuff));
	SelectObject(_DC, hOldFont);
	DeleteObject(hFont);
	SetTextColor(_DC, oldFontColor);
	SetBkMode(_DC, OPAQUE);
}

void CUIGold::Release()
{
}

void CUIGold::Key_Check(float _fdTime)
{
}
