#include "../stdafx.h"
#include "FButton.h"
#include "../MyBitmap/BmpMgr.h"

CFButton::CFButton()
	: m_bEnable(false)
{
	ZeroMemory(&m_tHitInfo, sizeof(INFO));
	ZeroMemory(&m_tHitRect, sizeof(RECT));
}


CFButton::~CFButton()
{
	Release();
}

bool CFButton::Initialize()
{
	m_tInfo.iCX = 28;
	m_tInfo.iCY = 26;
	if (OBJID::END == m_eID)
		return false;

	m_eRenderGroupID = GROUPID::UI;
	return true;
}

void CFButton::Key_Check(float _fdTime)
{
}

int CFButton::Update(float _fdTime)
{
	if (m_bEnable)
		Update_Rect();
	return OBJ_NOEVENT;
}

void CFButton::Late_Update(float _fdTime)
{
	if (m_bEnable)
		Update_Rect();
}

void CFButton::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	Update_Rect();
	if (m_bEnable) {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image("fButton");

		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY)
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, 0
			, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, MAGENTA_COLOR);
	}
}

void CFButton::Release()
{
}
