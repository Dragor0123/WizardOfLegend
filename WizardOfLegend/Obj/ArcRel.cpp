#include "../stdafx.h"
#include "ArcRel.h"

CArcRel::CArcRel()
	: m_bCool(false), m_fCoolTime(0.f), m_fCoolTLimit(5.f),
	m_bDead(false), m_pTarget(nullptr), m_strFrameKey(""),
	m_bOutter(false), m_bInner(false), 
	m_bSigniture(false), m_fSigEffectTime(0.f), m_fSigEffectTLimit(0.85f)
{
}

CArcRel::~CArcRel()
{
	Release();
}

bool CArcRel::Initialize()
{
	return true;
}

int CArcRel::Update(float _fdTime)
{
	return OBJ_NOEVENT;
}

void CArcRel::Late_Update(float _fdTime)
{
	if (m_fCoolTime > m_fCoolTLimit)
	{
		m_fCoolTime = 0.f;
		m_bCool = false;
	}
	if (!m_bInner && !m_bOutter)
		m_bDead = true;
}

void CArcRel::Release()
{
}

float CArcRel::Get_Remained_Cool() const
{
	float f = m_fCoolTLimit - m_fCoolTime;
	if (f < 0.f)
		return 0.f;
	else
		return f;
}

float CArcRel::Get_Remained_Cool_Ratio() const
{
	return m_fCoolTime / m_fCoolTLimit;
}
