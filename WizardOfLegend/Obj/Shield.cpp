#include "../stdafx.h"
#include "Shield.h"


CShield::CShield()
	: m_fDis(0.f), m_iAtt(1),
	m_fLifeTime(0.f),  m_fLifeTLimit(10.f)
{
}


CShield::~CShield()
{
	Release();
}

bool CShield::Initialize()
{
	m_fSpeed = 300.f;
	m_fDis = 140.f;
	Equalize_HitPosInfoPos();
	return true;
}

int CShield::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fLifeTime += _fdTime;
	m_fAngle += m_fSpeed * _fdTime;
	m_tInfo.fX = m_pTarget->Get_Info().fX + cosf(Degree_To_Radian(m_fAngle)) * m_fDis;
	m_tInfo.fY = m_pTarget->Get_Info().fY - sinf(Degree_To_Radian(m_fAngle)) * m_fDis;
	Equalize_HitPosInfoPos();
	Update_Rect();
	Update_HitRect();
	return OBJ_NOEVENT;
}

void CShield::Late_Update(float _fdTime)
{
	if (m_fLifeTime > m_fLifeTLimit)
		m_bDead = true;
	if (m_fAngle > 360.f)
		m_fAngle = 0.f;
}

void CShield::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
}

void CShield::Release()
{
}

void CShield::Move_Frame()
{
	CObj::Move_Frame();
}

void CShield::Scene_Change()
{
}
