#include "stdafx.h"
#include "Ref.h"

CRef::CRef()
	: m_iRef(1), m_strTag("")
{
}

CRef::CRef(const CRef & _other)
	: m_iRef(1), m_strTag(_other.m_strTag)
{
}

CRef::~CRef()
{
}
