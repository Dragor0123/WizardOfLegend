#include "../stdafx.h"
#include "Meteor.h"
#include "../MyBitmap/BmpMgr.h"
#include "UnderDeco.h"
#include "ObjMgr.h"
#include "../Manager/SoundMgr.h"

using namespace Meteor_Space;

CMeteor::CMeteor()
	: m_fCreateTime(0.f), m_bCircleInit(false), m_pCastingCircle(nullptr)
{
}


CMeteor::~CMeteor()
{
}

bool CMeteor::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Monster/FireBoss/Meteor.bmp", "Meteor"))
		return false;
	if (m_strFrameKey == "")
		m_strFrameKey = string("Meteor");

	m_tInfo.iCX = 480;
	m_tInfo.iCY = 792;

	m_tHitInfo.iCX = 0;
	m_tHitInfo.iCY = 0;

	m_tHitInfo.fX = m_tInfo.fX;
	m_tHitInfo.fY = m_tInfo.fY + 242.f;

	m_tStartPt.fX = m_tHitInfo.fX;
	m_tStartPt.fY = m_tHitInfo.fY;

	m_fShotRange = 600.f; // 1000.f
	m_fSpeed = 1200.f; //1200.f;
	m_iAtt = rand() % 15 + 28;
	m_fCreateTime = 0.f;

	m_ePreState = END;
	m_eCurState = END;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = METEOR_FIRE_FRAMESPEED;
	m_tFrame.dwFrameTime = GetTickCount();

	m_bCollision = false;
	return true;
}

int CMeteor::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bCircleInit)
	{
		m_pCastingCircle = CAbstractFactory<CUnderDeco>::Create(
			m_tStartPt.fX, m_tStartPt.fY + m_fShotRange, "CastingCircle");
		dynamic_cast<CUnderDeco*>(m_pCastingCircle)->Set_DrawID(1);
		m_pCastingCircle->Set_Target(this);
		CObjMgr::Get_Instance()->Add_Object(OBJID::UNDERDECO, m_pCastingCircle);
		STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
		PLAY_SOUND(L"MeteorFire.wav", CSoundMgr::MONSTER_EFFECT);
		m_bCircleInit = true;
	}
	
	if (m_bCircleInit && !m_bCollision)
		m_fCreateTime += _fdTime;

	if (m_fCreateTime > METEOR_CREATE_TIMELIMT && m_ePreState != CBullet::COLLISION && m_eCurState != CBullet::COLLISION)
	{
		m_eCurState = CBullet::FIRE;
	}

	if (m_tHitInfo.fY > m_tStartPt.fY + m_fShotRange + fY_OFFSET * 0.5f)
	{
		m_bCollision = true;
		m_tFrame.iFrameEnd = 8;
		m_fSpeed = 0.f;
	}

	if (m_ePreState == FIRE)
	{
		m_tInfo.fY += m_fSpeed * _fdTime;
		m_tHitInfo.fY += m_fSpeed * _fdTime;
	}

	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();

	return OBJ_NOEVENT;
}

void CMeteor::Late_Update(float _fdTime)
{
	if ((m_tHitInfo.fY > m_tStartPt.fY + m_fShotRange - fY_OFFSET * 1.5f) &&
		(m_tHitInfo.fY < m_tStartPt.fY + m_fShotRange + fY_OFFSET * 0.5f))
	{
		m_tHitInfo.iCX = iEXPLOSION_CX;
		m_tHitInfo.iCY = iEXPLOSION_CX;
	}

	if (m_bCollision)
	{
		m_eCurState = CBullet::COLLISION;
		m_eRenderGroupID = GROUPID::EFFECT;
		m_tFrame.iFrameEnd = 8;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
		if (m_pCastingCircle)
			m_pCastingCircle->Set_Dead();
	}

	Update_Rect();
	Update_HitRect();
}

void CMeteor::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	
	if (CBullet::END == m_ePreState)
		return;

	if (CBullet::FIRE == m_ePreState)
	{
		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY)
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * m_tInfo.iCX
			, m_tFrame.iFrameScene * m_tInfo.iCY
			, m_tInfo.iCX, m_tInfo.iCY
			, RGB(255, 255, 255));

	}
	else
	{
		GdiTransparentBlt(_DC
			, (int)(m_tRect.left + _fScrollX)
			, (int)(m_tRect.top + _fScrollY + 212)
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * m_tInfo.iCX
			, m_tFrame.iFrameScene * m_tInfo.iCY
			, m_tInfo.iCX, m_tInfo.iCY
			, RGB(255, 255, 255));
	}

	Draw_HitBox(_DC, _fScrollX, _fScrollY);

}

void CMeteor::Release()
{
}

void CMeteor::Move_Frame()
{
	if (CBullet::END == m_ePreState)
		return;

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == CBullet::FIRE) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 1;
		}
		else if (m_ePreState == CBullet::COLLISION) {
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				m_bDead = true;
			}
		}
	}
}

void CMeteor::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::FIRE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = METEOR_FIRE_FRAMESPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			//STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
			//PLAY_SOUND(L"MeteorFire.wav", CSoundMgr::MONSTER_EFFECT);
			break;
		case CBullet::COLLISION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = METEOR_COLLISION_FRAMESPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			STOP_SOUND(CSoundMgr::MONSTER_EFFECT);
			PLAY_SOUND(L"FireSkillEnd.wav", CSoundMgr::MONSTER_EFFECT);
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

int CMeteor::Get_Collision_Code() const
{
	return CC_MBULLET_NWALL_NPUSH;
}

void CMeteor::Change_HitRect()
{
}
