#pragma once

#ifndef __TELECIRCLE_H__
#define __TELECIRCLE_H__

#include "FAble.h"

class CTeleCircle : public CFAble
{
public:
	CTeleCircle();
	virtual ~CTeleCircle();

public:
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	// Inherited via CFAble
	virtual void Do_FButton_Action(float _fdTime);
	virtual int Get_Collision_Code() const { return CC_FABLE_TELECIRCLE; }

public:
	void Set_TeleEffectEnd() { m_bEffectEnd = true; }

private:
	void Change_Scene();

private:
	bool		m_bEffectEnd;
};

#endif