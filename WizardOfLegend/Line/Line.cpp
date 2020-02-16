#include "../stdafx.h"
#include "Line.h"

CLine::CLine()
{
}

CLine::CLine(const LINEPOS & _tLeft, const LINEPOS & _tRight)
	: m_tInfo(_tLeft, _tRight)
{
}

CLine::CLine(const LINEINFO & _tLineInfo)
	: m_tInfo(_tLineInfo)
{
}

CLine::~CLine()
{
}

void CLine::Render(HDC _DC, float _fScrollX, float _fScrollY)
{
	MoveToEx(_DC, (int)(m_tInfo.tLeftPos.fX + _fScrollX), (int)(m_tInfo.tLeftPos.fY + _fScrollY), NULL);
	LineTo(_DC, (int)(m_tInfo.tRightPos.fX + _fScrollX), (int)(m_tInfo.tRightPos.fY + _fScrollY));
}
