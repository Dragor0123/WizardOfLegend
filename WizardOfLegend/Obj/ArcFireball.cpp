#include "../stdafx.h"
#include "ArcFireball.h"
#include "FireBall.h"
#include "ObjMgr.h"
#include "Player.h"

const int CArcFireball::iSIG_BULLETMAX = 6;

// Set_Skill_Code(-1);

CArcFireball::CArcFireball()
	: m_iBulletCount(1), m_bInitFire(false), m_fBulletTick(0.125f),
	m_fAngle(0.f)
{
}


CArcFireball::~CArcFireball()
{
	Release();
}

bool CArcFireball::Initialize()
{
	if (!m_pTarget)
		return false;
	if (m_strFrameKey != "FireBall")
		m_strFrameKey = "FireBall";

	m_fCoolTLimit = 1.8f;
	return true;
}

int CArcFireball::Update(float _fdTime)
{
	if (m_bDead)
		return ARCRELIC_DROPPED;

	if (m_bInitFire)
	{
		m_fBulletTick += _fdTime;
		if (m_fBulletTick >= 0.125f)
		{
			m_fBulletTick = 0.f;
			Create_Bullet();
			--m_iBulletCount;
			if (m_iBulletCount <= 0)
			{
				m_bCool = true;
				m_bInitFire = false;
				m_iBulletCount = 1;
			}
		}
	}

	if (m_bCool)
	{
		m_fCoolTime += _fdTime;
		return ARCRELIC_COOLING;
	}

	return ARCRELIC_IDLE;
}

void CArcFireball::Late_Update(float _fdTime)
{
	if (m_fCoolTime > m_fCoolTLimit)
	{
		m_fCoolTime = 0.f;
		m_bCool = false;
	}
	if (!m_bInner && !m_bOutter)
		m_bDead = true;
}

void CArcFireball::Fire_Skill()
{
	if (!m_bOutter)
		return;

	if (!m_bCool)
	{
		if (!m_bInitFire) {
			m_fAngle = m_pTarget->Get_Angle();
			if ((0.f <= m_fAngle && m_fAngle < 45.f) || (315.f <= m_fAngle && m_fAngle < 360.f)) {
				m_pTarget->Set_FrameKey("Player_Right");
			}
			else if (45.f <= m_fAngle && m_fAngle < 135.f) {
				m_pTarget->Set_FrameKey("Player_Up");
			}
			else if (135.f <= m_fAngle && m_fAngle < 225.f) {
				m_pTarget->Set_FrameKey("Player_Left");
			}
			else {
				m_pTarget->Set_FrameKey("Player_Down");
			}
			m_bInitFire = true;

			static_cast<CPlayer*>(m_pTarget)->Set_PlayerState(CPlayer::ATTACK);
			static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
		}
	}
}

void CArcFireball::Key_Up_Action()
{
}

void CArcFireball::Passive_Effect()
{
}

CObj * CArcFireball::Create_Bullet()
{
	float _fX = 0.f;
	float _fY = 0.f;

	_fX = m_pTarget->Get_HitInfo().fX + cosf(Degree_To_Radian(m_fAngle)) * 72.f;
	_fY = m_pTarget->Get_HitInfo().fY - sinf(Degree_To_Radian(m_fAngle)) * 72.f;

	CObj* pBullet = CAbstractFactory<CFireBall>::Create(_fX, _fY,
		m_fAngle, "FireBall");
	static_cast<CBullet*>(pBullet)->Set_bMonsters(false);
	CObjMgr::Get_Instance()->Add_Object(OBJID::P_RECTBULLET, pBullet);
	return pBullet;
}

void CArcFireball::Release()
{
}
