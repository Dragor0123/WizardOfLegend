#include "../stdafx.h"
#include "EarthDrill.h"
#include "../MyBitmap/BmpMgr.h"
#include "EarthLoad.h"

CEarthDrill::CEarthDrill()
{
	m_bMonsters = true;
}


CEarthDrill::~CEarthDrill()
{
	Release();
}

bool CEarthDrill::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/EarthBoss_Drill.bmp", "EarthDrill"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/rot160.bmp", "EDrill_Rot"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/empty160.bmp", "EDrill_Empty"))
		return false;

	m_fSpeed = 0.f;
	m_tInfo.iCX = 160;
	m_tInfo.iCY = 160;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 126;
	m_tHitInfo.iCY = 93;

	m_iAtt = rand() % 7 + 11;

	m_strFrameKey = "EarthDrill";

	m_ePreState = CBullet::END;
	m_eCurState = CBullet::FIRE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_eRenderGroupID = GROUPID::GAMEOBJECT_2;
	return true;
}

int CEarthDrill::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_pTarget)
		return OBJ_DEAD;

	if (dynamic_cast<CEarthLoad*>(m_pTarget)) {
		m_tInfo.fX = (float)(static_cast<CEarthLoad*>(m_pTarget)->Get_PosinPt().x);
		m_tInfo.fX = (float)(static_cast<CEarthLoad*>(m_pTarget)->Get_PosinPt().y);
	}

	Equalize_HitPosInfoPos();
	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CEarthDrill::Late_Update(float _fdTime)
{
	if (m_bCollision) {
		m_eCurState = CBullet::COLLISION;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}
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

void CEarthDrill::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("EDrill_Rot");
	HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("EDrill_Empty");

	PlgBlt(hRotDC, m_tRotPoint,
		hMemDC,
		m_tFrame.iFrameStart * m_tInfo.iCX,
		0,
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

void CEarthDrill::Release()
{
}

void CEarthDrill::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			if (m_ePreState == CBullet::FIRE)
				m_tFrame.iFrameStart = 0;
			if (m_ePreState == CBullet::COLLISION)
				m_bDead;
		}
	}
}

void CEarthDrill::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
			case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
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

int CEarthDrill::Get_Collision_Code() const
{
	if (m_bMonsters)
		return CC_MBULLET_NWALL_NPUSH;
	else
		return CC_MBULLET_NWALL_PUSH;
}

void CEarthDrill::Change_HitRect()
{
	m_tHitInfo.iCX = (int)(abs(126 * cosf(Degree_To_Radian(m_fAngle))) + abs(93 * cosf(Degree_To_Radian(90.f + m_fAngle))));
	m_tHitInfo.iCY = (int)(abs(126 * sinf(Degree_To_Radian(m_fAngle))) + abs(93 * sinf(Degree_To_Radian(90.f + m_fAngle))));
}
