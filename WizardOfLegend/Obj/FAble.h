#pragma once

#ifndef __FABLE_H__
#define __FABLE_H__

#include "StopObj.h"

class CObj;

class CFAble : public CStopObj
{
public:
	CFAble();
	virtual ~CFAble();

	// Inherited via CStopObj
	virtual bool Initialize() override;
	virtual void Key_Check(float _fdTime) override;
	virtual int Update(float _fdTime) override;
	virtual void Late_Update(float _fdTime) override;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) override;
	virtual void Release() override;

public:
	virtual void Do_FButton_Action(float _fdTime) = 0;
	virtual int Get_Collision_Code() const { return CC_FABLE; }
	void Set_fButton(CObj* _fButton) { m_pFbutton = _fButton; }
	
	void Enable_FButton();
	void Disable_FButton();

protected:
	CObj*		m_pFbutton;
};

#endif