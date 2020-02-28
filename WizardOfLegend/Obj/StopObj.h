#pragma once

#ifndef __STOPOBJ_H__
#define __STOPOBJ_H__
#include "Obj.h"

class CStopObj : public CObj
{
public:
	CStopObj();
	virtual ~CStopObj();

	// Inherited via CObj
	virtual bool Initialize() = 0;
	virtual void Key_Check(float _fdTime) = 0;
	virtual int Update(float _fdTime) = 0;
	virtual void Late_Update(float _fdTime) = 0;
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f) = 0;
	virtual void Release() = 0;

};

#endif