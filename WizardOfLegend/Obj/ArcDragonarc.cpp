#include "../stdafx.h"
#include "ArcDragonarc.h"
#include "ScrewBullet.h"
#include "ObjMgr.h"
#include "Player.h"
#include "../Manager/SoundMgr.h"

const int CArcDragonarc::iBULLETMAX = 12;
const int CArcDragonarc::iSIG_BULLETMAX = 36;

CArcDragonarc::CArcDragonarc()
	: m_iBulletCount(iBULLETMAX), m_fBulletTick(0.f),
	m_bInitFire(false), m_fAngle(0.f)
{
}

CArcDragonarc::~CArcDragonarc()
{
	Release();
}

bool CArcDragonarc::Initialize()
{
	if (!m_pTarget)
		return false;
	if (m_strFrameKey != "DragonArc")
		m_strFrameKey = "DragonArc";

	m_fCoolTLimit = 4.8f;
	return true;
}

int CArcDragonarc::Update(float _fdTime)
{
	if (m_bDead)
		return ARCRELIC_DROPPED;

	if (m_bInitFire)
	{
		if (!m_bSigniture)
		{
			m_fBulletTick += _fdTime;
			if (m_fBulletTick >= 0.15f)
			{
				m_fBulletTick = 0.f;
				// Create_Bullet
				Create_Bullet(false);
				Create_Bullet(true);
				m_iBulletCount -= 2;
				if (m_iBulletCount <= 0)
				{
					m_bCool = true;
					m_bInitFire = false;
					static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
					m_iBulletCount = iBULLETMAX;
				}
			}
		}
		else // m_bSigniture == true
		{
			m_fSigEffectTime += _fdTime;

			if (m_fSigEffectTime > m_fSigEffectTLimit)
			{
				if (true == static_cast<CPlayer*>(m_pTarget)->Get_Signiture())
					static_cast<CPlayer*>(m_pTarget)->Set_Signiture(false);
				m_fBulletTick += _fdTime;
				if (m_fBulletTick >= 0.15f)
				{
					m_fBulletTick = 0.f;
					Create_SigBullet(false, m_fAngle + 30.f);
					Create_SigBullet(true, m_fAngle + 30.f);
					Create_SigBullet(false, m_fAngle);
					Create_SigBullet(true, m_fAngle);
					Create_SigBullet(false, m_fAngle - 30.f);
					Create_SigBullet(true, m_fAngle - 30.f);

					m_iBulletCount -= 6;
					if (m_iBulletCount <= 0)
					{
						m_bCool = true;
						m_bInitFire = false;
						m_bSigniture = false;
						m_fSigEffectTime = 0.f;
						static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
						m_iBulletCount = iBULLETMAX;
					}
				}
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

void CArcDragonarc::Late_Update(float _fdTime)
{
	if (m_fCoolTime > m_fCoolTLimit)
	{
		m_fCoolTime = 0.f;
		m_bCool = false;
	}

	if (!m_bInner && !m_bOutter)
		m_bDead = true;
}

void CArcDragonarc::Fire_Skill()
{
	if (!m_bOutter)
		return;

	if (!m_bCool)
	{
		if (!m_bInitFire)
		{ // 일반 공격
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
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
				PLAY_SOUND(L"Player_Ult_Use.wav", CSoundMgr::EFFECT);
				static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(1001);
				m_iBulletCount = iSIG_BULLETMAX;
			}
			else
			{
				static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(1);
			}

			static_cast<CPlayer*>(m_pTarget)->Set_PlayerState(CPlayer::ATTACK);
		}
	}
}


CObj * CArcDragonarc::Create_Bullet(bool _bClockwise)
{
	CObj* pBullet = nullptr;
	pBullet = CAbstractFactory<CScrewBullet>::Create(
		m_pTarget->Get_HitInfo().fX, m_pTarget->Get_HitInfo().fY,
		m_fAngle, "DragonArc");
	static_cast<CScrewBullet*>(pBullet)->Set_Rotation_Dir(_bClockwise);
	CObjMgr::Get_Instance()->Add_Object(OBJID::P_RECTBULLET, pBullet);
	return pBullet;
}

CObj * CArcDragonarc::Create_SigBullet(bool _bClockwise, float _fAngle)
{
	CObj* pBullet = nullptr;
	pBullet = CAbstractFactory<CScrewBullet>::Create(
		m_pTarget->Get_HitInfo().fX, m_pTarget->Get_HitInfo().fY,
		_fAngle, "DragonArc");
	static_cast<CBullet*>(pBullet)->Set_Signiture(true);
	static_cast<CScrewBullet*>(pBullet)->Set_Rotation_Dir(_bClockwise);
	static_cast<CBullet*>(pBullet)->Set_Att(int(static_cast<CBullet*>(pBullet)->Get_Att() * 3.5f));
	CObjMgr::Get_Instance()->Add_Object(OBJID::P_RECTBULLET, pBullet);
	return pBullet;
}

void CArcDragonarc::Key_Up_Action()
{
}

void CArcDragonarc::Passive_Effect()
{
}

void CArcDragonarc::Release()
{
}
