#include "../stdafx.h"
#include "ArcSphere.h"
#include "IceSphere.h"
#include "ObjMgr.h"
#include "Player.h"

// Set_Skill_Code(3);, Set_Skill_Code(-1);

CArcSphere::CArcSphere()
	: m_bShot(false), m_pLastMadeBullet(nullptr)
{
}

CArcSphere::~CArcSphere()
{
	Release();
}

bool CArcSphere::Initialize()
{
	if (!m_pTarget)
		return false;
	if (m_strFrameKey != "IceSphere")
		m_strFrameKey = "IceSphere";

	m_fCoolTLimit = 1.f;
	return true;
}

int CArcSphere::Update(float _fdTime)
{
	if (m_bDead)
		return ARCRELIC_DROPPED;

	if (m_bCool)
	{
		m_fCoolTime += _fdTime;
		return ARCRELIC_COOLING;
	}
	else
		return ARCRELIC_IDLE;
}

void CArcSphere::Late_Update(float _fdTime)
{
	if (m_fCoolTime > m_fCoolTLimit)
	{
		m_fCoolTime = 0.f;
		m_bCool = false;
	}
	if (!m_bInner && !m_bOutter)
		m_bDead = true;
}

void CArcSphere::Release()
{
}

void CArcSphere::Fire_Skill()
{
	if (!m_bOutter)
		return;

	if (!m_bCool)
	{
		float _fPlayerAngle = m_pTarget->Get_Angle();
		if ((0.f <= _fPlayerAngle && _fPlayerAngle < 45.f) || (315.f <= _fPlayerAngle && _fPlayerAngle < 360.f)) {
			m_pTarget->Set_FrameKey("Player_Right");
		}
		else if (45.f <= _fPlayerAngle && _fPlayerAngle < 135.f) {
			m_pTarget->Set_FrameKey("Player_Up");
		}
		else if (135.f <= _fPlayerAngle && _fPlayerAngle < 225.f) {
			m_pTarget->Set_FrameKey("Player_Left");
		}
		else {
			m_pTarget->Set_FrameKey("Player_Down");
		}
		dynamic_cast<CPlayer*>(m_pTarget)->Set_PlayerState(CPlayer::ATTACK);

		if (!m_bShot)
		{
			m_pLastMadeBullet = Create_Bullet();
#ifdef _DEBUG
			assert(m_pLastMadeBullet);
#endif
			m_bShot = true;
			static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(3);
		}
	}
}

void CArcSphere::Key_Up_Action()
{
	if (m_bShot &&
		!static_cast<CIceSphere*>(m_pLastMadeBullet)->Get_bFireStart())
	{
		static_cast<CIceSphere*>(m_pLastMadeBullet)->Set_FireStartOn();
		m_bShot = false;
		m_bCool = true;
		static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
	}
}

void CArcSphere::Passive_Effect()
{
}

CObj * CArcSphere::Create_Bullet()
{
	CObj* pSphere  = nullptr;
	pSphere = CAbstractFactory<CIceSphere>::Create(
		m_pTarget->Get_PosX(), m_pTarget->Get_PosY(), "IceSphere");

	if (pSphere) {
		pSphere->Set_Target(m_pTarget);
		CObjMgr::Get_Instance()->Add_Object(OBJID::P_RECTBULLET, pSphere);
	}
	return pSphere;
}