#include "../stdafx.h"
#include "ArcGaia.h"
#include "GaiaShield.h"
#include "ObjMgr.h"
#include "Player.h"
#include "../Manager/SoundMgr.h"

CArcGaia::CArcGaia()
{
}

CArcGaia::~CArcGaia()
{
	Release();
}

bool CArcGaia::Initialize()
{
	if (!m_pTarget)
		return false;
	if (m_strFrameKey != "GaiaShield")
		m_strFrameKey = "GaiaShield";


	//m_fCoolTLimit = 1.0f;
	m_fCoolTLimit = 8.0f;
	return true;
}

int CArcGaia::Update(float _fdTime)
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

void CArcGaia::Late_Update(float _fdTime)
{
	if (m_fCoolTime > m_fCoolTLimit)
	{
		m_fCoolTime = 0.f;
		m_bCool = false;
	}
	if (!m_bInner && !m_bOutter)
		m_bDead = true;
}

void CArcGaia::Release()
{
	// 패시브 효과 제거!
}

void CArcGaia::Fire_Skill()
{
	if (!m_bOutter)
		return;

	if (!m_bCool)
	{
		STOP_SOUND(CSoundMgr::EFFECT);
		PLAY_SOUND(L"GAIA_ARMOR_START.wav", CSoundMgr::EFFECT);
		for (int i = 0; i < 12; ++i)
		{
			Create_Shield((float)i * 30.f);
			static_cast<CPlayer*>(m_pTarget)->Set_Skill_Code(-1);
		}
		m_bCool = true;
	}
}

void CArcGaia::Key_Up_Action()
{
}

void CArcGaia::Passive_Effect()
{
}

CObj* CArcGaia::Create_Shield(float _fAngle)
{
	CObj* pObj = nullptr;
	pObj = CAbstractFactory<CGaiaShield>::Create(
		m_pTarget->Get_PosX(), m_pTarget->Get_PosY(),
		_fAngle, m_strFrameKey);

	if (pObj) {
		pObj->Set_Target(m_pTarget);
		CObjMgr::Get_Instance()->Add_Object(OBJID::P_SHIELD, pObj);
	}
	return pObj;
}