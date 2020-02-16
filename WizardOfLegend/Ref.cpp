#include "stdafx.h"
#include "Ref.h"


CRef::CRef()
	: m_iRef(1), m_bEnable(true), m_bDead(true)
{
}

CRef::CRef(const CRef & _other)
	: m_iRef(1), m_bEnable(_other.m_bEnable), m_bDead(_other.m_bDead)
{
}


CRef::~CRef()
{
}
