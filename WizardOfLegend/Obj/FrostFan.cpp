#include "../stdafx.h"
#include "FrostFan.h"
#include "../MyBitmap/BmpMgr.h"

CFrostFan::CFrostFan()
	: m_bHit(false)
{
}


CFrostFan::~CFrostFan()
{
	Release();
}

bool CFrostFan::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/FrostFan88.bmp", "FrostFan"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/rot160.bmp", "rot160"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Skill/empty160.bmp", "empty160"))
		return false;

	CBullet::Initialize();
	m_tInfo.iCX = 160;
	m_tInfo.iCY = 160;
	Equalize_HitPosInfoPos();

	m_tHitInfo.iCX = 88;
	m_tHitInfo.iCY = 30;
	m_fShotRange = 5000.f;
	m_fSpeed = 1200.f;
	m_iAtt = rand() % 7 + 15;

	if (m_strFrameKey == "")
		m_strFrameKey = string("FrostFan");

	m_ePreState = END;
	m_eCurState = FIRE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	// 	m_eRenderGroupID = GROUPID::GAMEOBJECT_2;
	return true;
}

int CFrostFan::Update(float _fdTime)
{
	return CLineBullet::Update(_fdTime);
}

void CFrostFan::Late_Update(float _fdTime)
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

void CFrostFan::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (m_ePreState != CBullet::COLLISION)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
		HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("rot160");
		HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("empty160");

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
			, MAGENTA_COLOR);

		BitBlt(hRotDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, hEmpDC, 0, 0, SRCCOPY);

		Draw_HitBox(_DC, _fScrollX, _fScrollY);
	}
}

void CFrostFan::Release()
{
}

void CFrostFan::Move_Frame()
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

void CFrostFan::Scene_Change()
{
	CLineBullet::Scene_Change();
}

int CFrostFan::Get_Collision_Code() const
{
	return CC_PBULLET_WALL_NPUSH_NDRAG;
}

void CFrostFan::Change_HitRect()
{
	m_tHitInfo.iCX = (int)(abs(88 * cosf(Degree_To_Radian(m_fAngle))) + abs(30 * cosf(Degree_To_Radian(90.f + m_fAngle))));
	m_tHitInfo.iCY = (int)(abs(88 * sinf(Degree_To_Radian(m_fAngle))) + abs(30 * sinf(Degree_To_Radian(90.f + m_fAngle))));
}
