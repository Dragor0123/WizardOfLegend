#include "stdafx.h"
#include "Hittable.h"

CHittable::CHittable()
	: m_iHp(1), m_iMaxHp(1), m_eMez(MEZ::MZ_NONE), m_iHitDigitCnt(0)
{
}


CHittable::~CHittable()
{
}
