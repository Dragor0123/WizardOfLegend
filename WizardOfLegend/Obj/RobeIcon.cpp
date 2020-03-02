#include "../stdafx.h"
#include "RobeIcon.h"
#include "../MyBitmap/BmpMgr.h"

CRobeIcon::CRobeIcon()
	: m_iDrawID(0)
{
}


CRobeIcon::~CRobeIcon()
{
	Release();
}

bool CRobeIcon::Initialize()
{
	m_tInfo.iCX = 58;
	m_tInfo.iCY = 58;
	if (m_strFrameKey == "")
		m_strFrameKey = "robe_icon";

	m_eRenderGroupID = GROUPID::UI;
	Update_Rect();
	return true;
}

int CRobeIcon::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CRobeIcon::Late_Update(float _fdTime)
{
}

void CRobeIcon::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left)
		, (int)(m_tRect.top)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * m_tInfo.iCX, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);
}

void CRobeIcon::Release()
{
}

void CRobeIcon::Key_Check(float _fdTime)
{
}
