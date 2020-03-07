#include "../stdafx.h"
#include "GuideLBullet.h"
#include "../MyBitmap/BmpMgr.h"
#include "ObjMgr.h"

namespace GuideLBullet_space
{
	const float fLaunchLimit = 1.2f;
}

using GuideLBullet_space::fLaunchLimit;

CGuideLBullet::CGuideLBullet()
	: m_fLifeTime(0.f), m_fLaunchTime(0.f)
{
	m_bMonsters = true;
}

CGuideLBullet::~CGuideLBullet()
{
	Release();
}

bool CGuideLBullet::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/GuideSword.bmp", "EarthBoss_Sword"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/rot115.bmp", "ESWord_Rot"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/EarthBoss/empty115.bmp", "ESWord_Empty"))
		return false;

	if (m_strFrameKey == "")
		m_strFrameKey = "EarthBoss_Sword";

	if (m_strFrameKey == "EarthBoss_Sword")
	{
		m_tInfo.iCX = 115;
		m_tInfo.iCY = 115;
		Equalize_HitPosInfoPos();

		m_tHitInfo.iCX = 80;
		m_tHitInfo.iCY = 36;
		m_fSpeed = 530.f;
		m_iAtt = rand() % 10 + 16;
		m_ePreState = m_eCurState = CBullet::END;
	}

	//m_ePreState = CBullet::END;
	//m_eCurState = CBullet::FIRE;
	// m_ePreState = m_eCurState = CBullet::END;

	m_eRenderGroupID = GROUPID::GAMEOBJECT_2;
	return true;
}

int CGuideLBullet::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_pTarget) {
		if (m_bMonsters) {
#ifdef _DEBUG
			assert(!CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty());
#endif
			m_pTarget = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
		}
		else {
			m_pTarget = CObjMgr::Get_Instance()->Get_Target(this, OBJID::MONSTER);
		}
	}

	if (m_pTarget)
	{
		float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
		float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
		float fDis = sqrtf(fX * fX + fY * fY);
		float fRadian = acosf(fX / fDis);

		m_fAngle = fRadian * 180.f / PI;

		if (m_tInfo.fY < m_pTarget->Get_Info().fY)
			m_fAngle *= -1.f;

		if (m_bMonsters)
		{
			m_fLaunchTime += _fdTime;
			if (m_fLaunchTime > fLaunchLimit)
				m_eCurState = CBullet::FIRE;
		}
	}

	if (!m_bMonsters && m_pTarget)
		m_eCurState = CBullet::FIRE;
	
	if (CBullet::END == m_eCurState) {
		return OBJ_NOEVENT;
	}
	else 
	{
		m_fLifeTime += _fdTime;
		m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed * _fdTime;
		m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed * _fdTime;

		Equalize_HitPosInfoPos();
	}

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();

	return OBJ_NOEVENT;
}

void CGuideLBullet::Late_Update(float _fdTime)
{
	if (m_bCollision || (m_bMonsters && m_fLifeTime > G_BULLET_LIFETIME_LIMIT))
		m_bDead = true;

	if (m_pTarget->Get_Dead())
		m_pTarget = nullptr;

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

void CGuideLBullet::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	HDC hRotDC = CBmpMgr::Get_Instance()->Find_Image("ESWord_Rot");
	HDC hEmpDC = CBmpMgr::Get_Instance()->Find_Image("ESWord_Empty");

	PlgBlt(hRotDC, m_tRotPoint,
		hMemDC,
		0, 0,
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

	//BitBlt(hRotDC, 0, 0, m_tInfo.iCX, m_tInfo.iCY, hEmpDC, 0, 0, SRCCOPY);
	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CGuideLBullet::Release()
{
}

void CGuideLBullet::Move_Frame()
{
}

void CGuideLBullet::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			if (m_strFrameKey == "EarthBoss_Sword")
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iFrameScene = 0;
				m_tFrame.dwFrameSpeed = 100;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		case CBullet::COLLISION:
			if (m_strFrameKey == "EarthBoss_Sword")
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iFrameScene = 0;
				m_tFrame.dwFrameSpeed = 100;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

int CGuideLBullet::Get_Collision_Code() const
{
	if (m_bMonsters)
		return CC_MBULLET_WALL_NPUSH;
	else
		return CC_PBULLET_WALL_NPUSH_NDRAG;
}

void CGuideLBullet::Change_HitRect()
{
	m_tHitInfo.iCX = (int)(abs(80 * cosf(Degree_To_Radian(m_fAngle))) + abs(36 * cosf(Degree_To_Radian(90.f + m_fAngle))));
	m_tHitInfo.iCY = (int)(abs(80 * sinf(Degree_To_Radian(m_fAngle))) + abs(36 * sinf(Degree_To_Radian(90.f + m_fAngle))));
}
