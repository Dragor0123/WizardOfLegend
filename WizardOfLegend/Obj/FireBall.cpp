#include "../stdafx.h"
#include "FireBall.h"
#include "../MyBitmap/BmpMgr.h"

CFireBall::CFireBall()
	: m_fRisingTime(0.f), m_fRisingLimit(.76f)
{
	m_bMonsters = true;
}

CFireBall::~CFireBall()
{
	Release();
}

bool CFireBall::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/Fireball_Boss.bmp", "FireBoss_Fireball"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/rot200.bmp", "rot200"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/empty200.bmp", "empty200"))
		return false;

	CBullet::Initialize();
	m_tInfo.iCX = 200;
	m_tInfo.iCY = 200;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 136;
	m_tHitInfo.iCX = 92;

	m_fShotRange = 1200.f;
	m_fSpeed = 1100.f;

	if (m_strFrameKey == "")
		m_strFrameKey = string("FireBoss_Fireball");

	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	return true;
}

int CFireBall::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bMonsters && m_pTarget)
	{
		m_fRisingTime += _fdTime;
		if (m_fRisingTime <= m_fRisingLimit)
		{
			float fDX = 0.f, fDY = 0.f, fDis = 0.f, fRadian = 0.f;
			fDX = m_pTarget->Get_PosX() - m_tInfo.fX;
			fDY = m_pTarget->Get_PosY() - m_tInfo.fY;
			fDis = sqrtf(fDX * fDX + fDY * fDY);
			fRadian = acosf(fDX / fDis);

			if (m_tInfo.fY < m_pTarget->Get_Info().fY)
				fRadian = 2 * PI - fRadian;
			m_fAngle = fRadian * 180.f / PI;
		}
		else
		{
			m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
			m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
		}
	}
	else
	{
		m_tInfo.fX += cosf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
		m_tInfo.fY -= sinf(Degree_To_Radian(m_fAngle)) * m_fSpeed * _fdTime;
	}

	Equalize_HitPosInfoPos();

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CFireBall::Late_Update(float _fdTime)
{
	CBullet::Late_Update(_fdTime);
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

void CFireBall::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("rot200");
	HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("empty200");

	PlgBlt(hRotDC, m_tRotPoint,
		hMemDC,
		m_tFrame.iFrameStart * m_tInfo.iCX, 0,
		m_tInfo.iCX, m_tInfo.iCY,
		NULL, NULL, NULL);

	GdiTransparentBlt(_DC,
		(int)(m_tRect.left + _fScrollX),
		(int)(m_tRect.top + _fScrollY),
		m_tInfo.iCX, m_tInfo.iCY,
		hRotDC,
		0, 0,
		m_tInfo.iCX, m_tInfo.iCY,
		MAGENTA_COLOR);

	BitBlt(hRotDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, hEmpDC, 0, 0, SRCCOPY);
	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CFireBall::Release()
{
}

void CFireBall::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == CBullet::FIRE) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;
		}
		else if (m_ePreState == CBullet::COLLISION) {
			m_bDead = true;
		}
	}
}

void CFireBall::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
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

void CFireBall::Change_HitRect()
{
	m_tHitInfo.iCX = (int)(abs(136 * cosf(Degree_To_Radian(m_fAngle))) + abs(92 * cosf(Degree_To_Radian(90.f + m_fAngle))));
	m_tHitInfo.iCY = (int)(abs(136 * sinf(Degree_To_Radian(m_fAngle))) + abs(92 * sinf(Degree_To_Radian(90.f + m_fAngle))));
}

int CFireBall::Get_Collision_Code() const
{
	if (m_bMonsters)
		return CC_MBULLET_WALL_PUSH;
	else
		return CC_PBULLET_WALL_PUSH_NDRAG;
}
