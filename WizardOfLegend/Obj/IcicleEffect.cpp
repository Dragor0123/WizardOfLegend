#include "../stdafx.h"
#include "IcicleEffect.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Hittable.h"
#include "Player.h"
#include "Monster.h"

const float CIcicleEffect::m_fLifeTLimit = 3.6f;
CIcicleEffect::CIcicleEffect()
	: m_fLifeTime(0.f)
{
}


CIcicleEffect::~CIcicleEffect()
{
	Release();
}

bool CIcicleEffect::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/DecoEffect/Icicle.bmp", "Icicle"))
		return false;

	if (!m_pTarget)
		return false;

	m_tInfo.fX = m_pTarget->Get_PosX();
	m_tInfo.fY = m_pTarget->Get_PosY();
	m_tInfo.iCX = 150;
	m_tInfo.iCY = 150;

	if (!dynamic_cast<CHittable*>(m_pTarget))
		return false;

	dynamic_cast<CHittable*>(m_pTarget)->Set_MezState(MEZ::MZ_FROZEN);
	m_strFrameKey = "Icicle";
	m_eRenderGroupID = GROUPID::EFFECT;
	return true;
}

int CIcicleEffect::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	m_fLifeTime += _fdTime;
	Update_Rect();

	return OBJ_NOEVENT;
}

void CIcicleEffect::Late_Update(float _fdTime)
{
	if (m_pTarget->Get_Dead() || !m_pTarget)
		m_bDead = true;

	if (m_fLifeTime > m_fLifeTLimit)
	{
		m_bDead = true;
		dynamic_cast<CHittable*>(m_pTarget)->Set_MezState(MEZ::MZ_NONE);
		if (dynamic_cast<CPlayer*>(m_pTarget))
			static_cast<CPlayer*>(m_pTarget)->Set_PlayerState(CPlayer::IDLE);
		if (dynamic_cast<CMonster*>(m_pTarget))
			static_cast<CMonster*>(m_pTarget)->Set_Monster_State(CMonster::IDLE);
	}
}

void CIcicleEffect::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0
		, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);
}

void CIcicleEffect::Release()
{
}

void CIcicleEffect::Key_Check(float _fdTime)
{
}