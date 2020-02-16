#pragma once

#ifndef __LINE_H__
#define __LINE_H__

class CLine
{
public:
	CLine();
	CLine(const LINEPOS& _tLeft, const LINEPOS& _tRight);
	CLine(const LINEINFO & _tLineInfo);
	~CLine();

public:
	void Render(HDC _DC, float _fScrollX, float _fScrollY);

	const LINEINFO& Get_Info() const { return m_tInfo; }

private:
	LINEINFO	m_tInfo;
};

#endif