#include "../stdafx.h"
#include "Arrow.h"
#include "../MyBitmap/BmpMgr.h"

CArrow::CArrow()
{
}

CArrow::~CArrow()
{
	Release();
}

bool CArrow::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/Arrow.bmp", "Arrow"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/rot150.bmp", "rot150"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/Archer/empty150.bmp", "empty150"))
		return false;

	CBullet::Initialize();

	m_tInfo.iCX = 150;
	m_tInfo.iCY = 150;
	Equalize_HitPosInfoPos();
	
	m_tHitInfo.iCX = 74;
	m_tHitInfo.iCY = 16;

	m_fShotRange = 720.f;
	m_fSpeed = 900.f;
	m_iAtt = 20;

	if (m_strFrameKey == "")
		m_strFrameKey = string("Arrow");

	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	
	m_eRenderGroupID = GROUPID::GAMEOBJECT_2;
	return true;
}

int CArrow::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
	m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;

	Equalize_HitPosInfoPos();

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CArrow::Late_Update(float _fdTime)
{
	if (m_bCollision) {
		m_bDead = true;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}

	float fdX = 0.f, fdY = 0.f;
	fdX = m_tInfo.fX - m_tStartPt.fX;
	fdY = m_tInfo.fY - m_tStartPt.fY;

	float fdist = sqrtf(fdX * fdX + fdY * fdY);

	if (fdist > m_fShotRange)
		m_bDead = true;

	Update_Rect();
	Update_HitRect();

	float	fDis = sqrtf(float((m_tInfo.iCX >> 1) * (m_tInfo.iCX >> 1) + (m_tInfo.iCY >> 1) * (m_tInfo.iCY >> 1)));
	m_tRotPoint[0].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 135.f) * PI / 180.f) * fDis));
	m_tRotPoint[0].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 135.f) * PI / 180.f) * fDis));
	m_tRotPoint[1].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 45.f) * PI / 180.f) * fDis));
	m_tRotPoint[1].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 45.f) * PI / 180.f) * fDis));
	m_tRotPoint[2].x = LONG((m_tInfo.iCX >> 1) + (cosf((m_fAngle + 225.f) * PI / 180.f) * fDis));
	m_tRotPoint[2].y = LONG((m_tInfo.iCY >> 1) - (sinf((m_fAngle + 225.f) * PI / 180.f) * fDis));
}

void CArrow::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (m_ePreState != CBullet::COLLISION)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
		HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("rot150");
		HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("empty150");

		PlgBlt(hRotDC, m_tRotPoint,
			hMemDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY,
			NULL, NULL, NULL);

		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY)
			, m_tInfo.iCX, m_tInfo.iCY
			, hRotDC
			, 0
			, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, RGB(255, 0, 255));

		BitBlt(hRotDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, hEmpDC, 0, 0, SRCCOPY);

		Draw_HitBox(_DC, _fScrollX, _fScrollY);
	}
}

void CArrow::Release()
{
}

void CArrow::Move_Frame()
{
}

void CArrow::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CBullet::COLLISION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CArrow::Change_HitRect()
{
	m_tHitInfo.iCX = (int)(abs(74 * cosf(Degree_To_Radian(m_fAngle))) + abs(16 * cosf(Degree_To_Radian(90.f + m_fAngle))));
	m_tHitInfo.iCY = (int)(abs(74 * sinf(Degree_To_Radian(m_fAngle))) + abs(16 * sinf(Degree_To_Radian(90.f + m_fAngle))));
}
