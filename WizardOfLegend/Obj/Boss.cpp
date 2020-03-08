#include "../stdafx.h"
#include "Boss.h"


const int CBoss::B_HIT_FRAME_COUNTMAX = 7;
const int CBoss::B_DANCE_COUNTMAX = 1;
CBoss::CBoss()
	: m_bDanceCool(false), m_bHittable(false),
	m_iAttackCnt(0), m_iPatternCnt(0),
	m_ePreState(CBoss::S_END), m_eCurState(CBoss::IDLE),
	m_eDir(CBoss::BOTTOM)
{
}


CBoss::~CBoss()
{
	Release();
}

bool CBoss::Initialize()
{
	CMoveObj::Initialize();
	m_fSpeed = 750.f;
	m_ePreState = EARTHB_STATE::S_END;
	m_eCurState = EARTHB_STATE::IDLE;

	//m_iMaxHp = 2000;
	m_iMaxHp = 300;
	m_iHp = m_iMaxHp;
	m_fAngle = 0.f;
	m_fDetectRange = DEFAULT_DETECT_RAD * 1.07f;

	return true;
}

void CBoss::Late_Update(float _fdTime)
{
	if (0 >= m_iHp)
	{
		m_eCurState = CBoss::DEAD;
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}

	if (m_iHitDigitCnt > HIT_DIGIT_CNT_MAX || m_ePreState != CBoss::HIT)
		Reset_HitDigitCnt();

	Update_Rect();
	Update_HitRect();
}

void CBoss::Release()
{
}

void CBoss::Move_Frame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}
}
