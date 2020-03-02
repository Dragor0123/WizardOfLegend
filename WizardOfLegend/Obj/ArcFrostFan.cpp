#include "../stdafx.h"
#include "ArcFrostFan.h"
#include "FrostFan.h"
#include "ObjMgr.h"
#include "Player.h"

CArcFrostFan::CArcFrostFan()
	:m_iBulletCount(5), m_fBulletTick(0.f), m_bInitFire(false)
{
}


CArcFrostFan::~CArcFrostFan()
{
	Release();
}

bool CArcFrostFan::Initialize()
{
	if (!m_pTarget)
		return false;
	if (m_strFrameKey != "FrostFan")
		m_strFrameKey = "FrostFan";

	m_fCoolTLimit = 0.2f;
	return true;
}

int CArcFrostFan::Update(float _fdTime)
{
	if (m_bDead)
		return ARCRELIC_DROPPED;

	if (m_bInitFire)
	{
		m_fBulletTick += _fdTime;
		if (m_fBulletTick >= 0.0125f)
		{
			m_fBulletTick = 0.f;
			Create_Bullet(m_fAngle + m_iBulletCount * 8.f - 15.f);
			--m_iBulletCount;
			if (m_iBulletCount <= 0)
			{
				m_bCool = true;
				m_bInitFire = false;
				m_iBulletCount = 5;
			}
		}
	}

	if (m_bCool)
	{
		m_fCoolTime += _fdTime;
		return ARCRELIC_COOLING;
	}
	return OBJ_NOEVENT;
}

void CArcFrostFan::Late_Update(float _fdTime)
{
	if (m_fCoolTime > m_fCoolTLimit)
	{
		m_fCoolTime = 0.f;
		m_bCool = false;
	}
	if (!m_bInner && !m_bOutter)
		m_bDead = true;
}

void CArcFrostFan::Fire_Skill()
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
		}
		dynamic_cast<CPlayer*>(m_pTarget)->Set_PlayerState(CPlayer::ATTACK);
		// 그리고 Player에게 어떤 조건값? int코드값? 이런 걸 넘겨서 attack frame이 다르게 움직이도록... 설정해준다.
	}
}

CObj * CArcFrostFan::Create_Bullet(float _fAngle)
{
	CObj* pBullet = nullptr;
	pBullet = CAbstractFactory<CFrostFan>::Create(
		m_pTarget->Get_PosX(), m_pTarget->Get_PosY(), _fAngle, "FrostFan");

	if (pBullet)
		CObjMgr::Get_Instance()->Add_Object(OBJID::P_RECTBULLET, pBullet);

	return pBullet;
}


void CArcFrostFan::Key_Up_Action()
{
}

void CArcFrostFan::Passive_Effect()
{
}

void CArcFrostFan::Release()
{
}
