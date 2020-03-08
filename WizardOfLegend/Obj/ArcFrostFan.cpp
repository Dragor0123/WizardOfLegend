#include "../stdafx.h"
#include "ArcFrostFan.h"
#include "FrostFan.h"
#include "ObjMgr.h"
#include "Player.h"
#include "../Manager/SoundMgr.h"

// Set_Skill_Code(5);
const int CArcFrostFan::iBULLETMAX = 5;
const int CArcFrostFan::iSIG_CYCLEMAX = 3;
CArcFrostFan::CArcFrostFan()
	:m_iBulletCount(iBULLETMAX), m_fBulletTick(0.f), m_bInitFire(false),
	m_fAngle(0.f), m_fSigCycleTick(0.f), m_iSigCycleCount(iSIG_CYCLEMAX), m_bSigCycleTime(false)
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

	m_fCoolTLimit = 3.6f;
	return true;
}

int CArcFrostFan::Update(float _fdTime)
{
	if (m_bDead)
		return ARCRELIC_DROPPED;

	if (m_bInitFire)
	{
		if (!m_bSigniture)
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
					static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
					m_iBulletCount = iBULLETMAX;
				}
			}
		}
		else
		{
			m_fSigEffectTime += _fdTime;

			if (m_fSigEffectTime > m_fSigEffectTLimit)
			{
				if (true == static_cast<CPlayer*>(m_pTarget)->Get_Signiture())
					static_cast<CPlayer*>(m_pTarget)->Set_Signiture(false);
				
				if (m_bSigCycleTime)
					m_fSigCycleTick += _fdTime;

				if (m_fSigCycleTick > 0.2f)
				{
					m_bSigCycleTime = false;
					m_fSigCycleTick = 0.f;
				}

				if (!m_bSigCycleTime)
				{
					m_fBulletTick += _fdTime;
					if (m_fBulletTick >= 0.0125f)
					{
						m_fBulletTick = 0.f;
						if (m_iSigCycleCount > 1)
							Create_SigBullet(m_fAngle + m_iBulletCount * 8.f - 15.f, false);
						else
							Create_SigBullet(m_fAngle + m_iBulletCount * 8.f - 15.f, true);

						--m_iBulletCount;
						if (m_iBulletCount <= 0)
						{
							m_bSigCycleTime = true;
							m_iBulletCount = iBULLETMAX;
							--m_iSigCycleCount;
							if (m_iSigCycleCount <= 0)
							{
								m_bCool = true;
								m_bInitFire = false;
								m_bSigniture = false;
								m_fSigEffectTime = 0.f;
								static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
								m_iBulletCount = iBULLETMAX;

								m_fSigCycleTick = 0.f;
								m_bSigCycleTime = false;
								m_iSigCycleCount = iSIG_CYCLEMAX;
							}
						}
					}
				}
			}
		}
	} //	if (m_bInitFire)

	if (m_bCool)
	{
		m_fCoolTime += _fdTime;
		return ARCRELIC_COOLING;
	}
	return ARCRELIC_IDLE;
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
			if (static_cast<CPlayer*>(m_pTarget)->Is_MP_Max())
			{
				m_bSigniture = true;
				static_cast<CPlayer*>(m_pTarget)->Set_Signiture(true);
				static_cast<CPlayer*>(m_pTarget)->Init_Effect_Line();
				static_cast<CPlayer*>(m_pTarget)->Set_MP_Zero();
				// 스킬코드를 따로 주던지...
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
				PLAY_SOUND(L"Player_Ult_Use.wav", CSoundMgr::EFFECT);
				static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(1000 + 5);
				m_iBulletCount = iBULLETMAX;
			}
			else
			{
				static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(5);
			}
			static_cast<CPlayer*>(m_pTarget)->Set_PlayerState(CPlayer::ATTACK);
		}
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

CObj * CArcFrostFan::Create_SigBullet(float _fAngle, bool _bMakingIce)
{
	CObj* pBullet = nullptr;
	pBullet = CAbstractFactory<CFrostFan>::Create(
		m_pTarget->Get_PosX(), m_pTarget->Get_PosY(), _fAngle, "FrostFan");

	static_cast<CBullet*>(pBullet)->Set_Signiture(true);
	static_cast<CBullet*>(pBullet)->Set_Att(int(static_cast<CBullet*>(pBullet)->Get_Att() * 3.0f));
	static_cast<CFrostFan*>(pBullet)->Set_bMaking_Ice(_bMakingIce);
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
