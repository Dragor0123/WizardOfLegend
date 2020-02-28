#include "../stdafx.h"
#include "StopObj.h"

CStopObj::CStopObj()
{
}

CStopObj::~CStopObj()
{
}

bool CStopObj::Initialize()
{
	return true;
}

int CStopObj::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CStopObj::Late_Update(float _fdTime)
{
}

void CStopObj::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
}

void CStopObj::Release()
{
}

