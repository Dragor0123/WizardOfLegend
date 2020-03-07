#include "../stdafx.h"
#include "ArcFireball.h"
#include "FireBall.h"
#include "ObjMgr.h"
#include "Player.h"

const int CArcFireball::iSIG_BULLETMAX = 8;

CArcFireball::CArcFireball()
	: m_bInitFire(false), m_fAngle(0.f),
	m_iBulletCount(1), m_fBulletTick(0.125f)
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

	m_fCoolTLimit = 2.25f;
	return true;
}

int CArcFireball::Update(float _fdTime)
{
	if (m_bDead)
		return ARCRELIC_DROPPED;

	if (m_bInitFire)
	{
		if (!m_bSigniture)
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
					static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
					m_iBulletCount = 1;
				}
			}
		}
		else  // m_bSigniture == true
		{
			m_fSigEffectTime += _fdTime;
			if (m_fSigEffectTime > m_fSigEffectTLimit)
			{
				if (true == static_cast<CPlayer*>(m_pTarget)->Get_Signiture())
					static_cast<CPlayer*>(m_pTarget)->Set_Signiture(false);
				
				m_fBulletTick += _fdTime;
				if (m_fBulletTick >= 0.125f)
				{
					m_fBulletTick = 0.f;
					for (int i = 0; i < iSIG_BULLETMAX; ++i)
						Create_SigBullet( 45.f * i);

					m_iBulletCount -= iSIG_BULLETMAX;
					if (m_iBulletCount <= 0)
					{
						m_bCool = true;
						m_bInitFire = false;
						m_bSigniture = false;
						m_fSigEffectTime = 0.f;
						static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
						m_iBulletCount = 1;
					}
				}
			}
		}  // else  (m_bSigniture == true)
	} //	if (m_bInitFire)

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

			if (static_cast<CPlayer*>(m_pTarget)->Is_MP_Max())
			{
				m_bSigniture = true;
				static_cast<CPlayer*>(m_pTarget)->Set_Signiture(true);
				static_cast<CPlayer*>(m_pTarget)->Init_Effect_Line();
				static_cast<CPlayer*>(m_pTarget)->Set_MP_Zero();
				// 스킬코드를 따로 주던지...
				static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1 + 1000);
				m_iBulletCount = iSIG_BULLETMAX;
			}
			else
			{
				static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
			}

			static_cast<CPlayer*>(m_pTarget)->Set_PlayerState(CPlayer::ATTACK);
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

	CObj* pBullet = new CFireBall;
	pBullet->Set_Pos(_fX, _fY);
	pBullet->Set_Angle(m_fAngle);
	pBullet->Set_FrameKey("FireBall");
	static_cast<CBullet*>(pBullet)->Set_bMonsters(false);
	pBullet->Initialize();

	CObjMgr::Get_Instance()->Add_Object(OBJID::P_RECTBULLET, pBullet);
	
	return pBullet;
}

CObj * CArcFireball::Create_SigBullet(float _fAngle)
{
	float _fX = 0.f;
	float _fY = 0.f;

	_fX = m_pTarget->Get_HitInfo().fX + cosf(Degree_To_Radian(_fAngle)) * 72.f;
	_fY = m_pTarget->Get_HitInfo().fY - sinf(Degree_To_Radian(_fAngle)) * 72.f;
	CObj* pBullet = new CFireBall;
	pBullet->Set_Pos(_fX, _fY);
	pBullet->Set_Angle(_fAngle);
	pBullet->Set_FrameKey("FireBall");
	static_cast<CBullet*>(pBullet)->Set_Signiture(true);
	static_cast<CBullet*>(pBullet)->Set_bMonsters(false);
	pBullet->Initialize();
	static_cast<CBullet*>(pBullet)->Set_Att(int(static_cast<CBullet*>(pBullet)->Get_Att() * 2.f));
	CObjMgr::Get_Instance()->Add_Object(OBJID::P_RECTBULLET, pBullet);
	return pBullet;
}

void CArcFireball::Release()
{
}
