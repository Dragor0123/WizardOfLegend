#pragma once

#ifndef __MYBUTTON_H__
#define __MYBUTTON_H__

#include "StopObj.h"

// Title Menu Button
class CMyButton : public CStopObj
{
public:
	CMyButton();
	virtual ~CMyButton();

	// Inherited via CStopObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

private:
	int			m_iDrawID;
};

#endif