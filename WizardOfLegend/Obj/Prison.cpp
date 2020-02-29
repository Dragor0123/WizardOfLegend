#include "../stdafx.h"
#include "Prison.h"
#include "../MyBitmap/BmpMgr.h"

CPrison::CPrison()
{
}

CPrison::~CPrison()
{
	Release();
}

bool CPrison::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Obstacle/Prison_Vertical.bmp", "Prison_V"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Obstacle/Prison_Horizon.bmp", "Prison_H"))
		return false;

	if (m_fAngle == 90.f)
	{
		m_strFrameKey = "Prison_V";
		m_tInfo.iCX = 48;
		m_tInfo.iCY = 192;
	}
	else if (m_fAngle == 0.f)
	{
		m_strFrameKey = "Prison_H";
		m_tInfo.iCX = 192;
		m_tInfo.iCY = 48;
	}
	else
	{
		return false;
	}

	Equalize_HitPosInfoPos();
	Equalize_HitSzInfoSz();

	Update_Rect();
	Update_HitRect();

	return true;
}

int CPrison::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CPrison::Late_Update(float _fdTime)
{
}

void CPrison::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CPrison::Release()
{
}

void CPrison::Key_Check(float _fdTime)
{
}
