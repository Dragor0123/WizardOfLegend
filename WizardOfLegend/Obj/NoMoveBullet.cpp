#include "../stdafx.h"
#include "NoMoveBullet.h"
#include "../MyBitmap/BmpMgr.h"
#include "UnderDeco.h"
#include "ObjMgr.h"
#include "../Manager/SoundMgr.h"

namespace NoMoveBul_Space{
	const float PILLAR_Y_OFFSET = 90;
	const float PILLAR_CREATE_TIME_LIMIT = 0.52f;
}

using NoMoveBul_Space::PILLAR_Y_OFFSET;
using NoMoveBul_Space::PILLAR_CREATE_TIME_LIMIT;

CNoMoveBullet::CNoMoveBullet()
	: m_fPillarCreateTime(0.f), m_bCrevisInit(false)
{
}

CNoMoveBullet::~CNoMoveBullet()
{
	Release();
}

bool CNoMoveBullet::Initialize()
{
	if (m_strFrameKey == "")
		return false;

	if (m_strFrameKey == "EarthBoss_Pillar")
	{
		m_tInfo.iCX = 64;
		m_tInfo.iCY = 180;

		m_tHitInfo.fX = m_tInfo.fX;
		m_tHitInfo.fY = m_tInfo.fY + (146.f - PILLAR_Y_OFFSET);

		m_tHitInfo.iCX = 48;
		m_tHitInfo.iCY = 50;

		m_ePreState = m_eCurState = CBullet::END;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 2;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = 120;
	}

	m_iAtt = rand() % 11 + 30;
	m_ePreState = m_eCurState = CBullet::END;
	return true;
}

int CNoMoveBullet::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bCrevisInit)
	{
		CObj* pCrevis = CAbstractFactory<CUnderDeco>::Create(m_tHitInfo.fX, m_tHitInfo.fY, "Crevis");
		CObjMgr::Get_Instance()->Add_Object(OBJID::DECO, pCrevis);
		m_bCrevisInit = true;
	}

	m_fPillarCreateTime += _fdTime;
	if (m_fPillarCreateTime > PILLAR_CREATE_TIME_LIMIT)
	{
		m_eCurState = CBullet::FIRE;
	}

	if (CBullet::END == m_eCurState)
		return OBJ_NOEVENT;

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CNoMoveBullet::Late_Update(float _fdTime)
{
	if (m_bCollision)
		m_bDead = true;
	Update_Rect();
	Update_HitRect();
}

void CNoMoveBullet::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	if (CBullet::END == m_ePreState)
		return;

	if (m_strFrameKey == "EarthBoss_Pillar")
	{
		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX,
			0,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);
	}

	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CNoMoveBullet::Release()
{
}

void CNoMoveBullet::Move_Frame()
{
	//if (CBullet::END == m_eCurState)
	//	return;
	if (CBullet::END == m_ePreState)
		return;
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();


		if (m_strFrameKey == "EarthBoss_Pillar")
		{
			if (m_tFrame.iFrameStart == 1)
				PLAY_SOUND(L"EarthSkillEnd.wav", CSoundMgr::MONSTER_EFFECT);
			else if (m_tFrame.iFrameStart == 2)
				STOP_SOUND(CSoundMgr::MONSTER_EFFECT);

			if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				m_tFrame.dwFrameSpeed = 1600;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				m_bDead = true;
			}
		}
	}
}

void CNoMoveBullet::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			if (m_strFrameKey == "EarthBoss_Pillar")
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 2;
				m_tFrame.iFrameScene = 0;
				m_tFrame.dwFrameSpeed = 120;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		case CBullet::COLLISION:
			if (m_strFrameKey == "EarthBoss_Pillar")
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iFrameScene = 0;
				m_tFrame.dwFrameSpeed = 120;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}

}

void CNoMoveBullet::Change_HitRect()
{
	if (CBullet::END == m_eCurState)
		return;

	if ("EarthBoss_Pillar" == m_strFrameKey)
	{
		if (m_tFrame.iFrameStart < 2)
		{
			m_tHitInfo.fX = m_tInfo.fX;
			m_tHitInfo.fY = m_tInfo.fY + (148.f - PILLAR_Y_OFFSET);
			m_tHitInfo.iCX = 48;
			m_tHitInfo.iCY = 50;
		}
		else if (m_tFrame.iFrameStart == 2)
		{
			m_tHitInfo.fX = m_tInfo.fX;
			m_tHitInfo.fY = m_tInfo.fY + (135.f - PILLAR_Y_OFFSET);
			m_tHitInfo.iCY = 76;
		}
		else {
			m_tHitInfo.iCX = 0;
			m_tHitInfo.iCY = 0;
		}
	}
}

int CNoMoveBullet::Get_Collision_Code() const
{
	if (m_strFrameKey == "EarthBoss_Pillar")
	{
		return  CC_MBULLET_WALL_PUSH;
	}
	else
	{
		return  CC_PBULLET_WALL_NPUSH_NDRAG;
	}
}
