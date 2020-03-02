#pragma once

#ifndef __DIGIT_H__
#define __DIGIT_H__

#include "MoveObj.h"

namespace Digit_space
{
	const int iDIG_WIDTH = 24;
}

class CDigit : public CMoveObj
{
public:
	CDigit();
	virtual ~CDigit();

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	void Set_LifeTime(bool _b) { m_bUsingLifeTime = _b; }
	void Set_Moving(bool _b) { m_bMoving = _b; }

	void Set_Number(int i) { m_iNumber = i; }
private:
	int						m_iNumber;				// drawID
	
	bool					m_bUsingLifeTime;
	bool					m_bMoving;
	float					m_fLifeTime;
	static const float		fLIFETIME_LIMIT;
	// int			iTemp;
};

#endif
