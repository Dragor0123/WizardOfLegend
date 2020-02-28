#pragma once

#ifndef __FBUTTON_H__
#define __FBUTTON_H__

#include "StopObj.h"
#include "FAble.h"

class CFButton : public CStopObj
{
	friend 	void CFAble::Enable_FButton();
	friend 	void CFAble::Disable_FButton();
public:
	CFButton();
	virtual ~CFButton();

	// Inherited via CStopObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	bool Is_Enable() { return m_bEnable; }
private:
	void Set_Enable(bool _b) { m_bEnable = _b; }

private:
	bool		m_bEnable;

};

#endif
