#pragma once

#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "MoveObj.h"

class CMouse : public CMoveObj
{
public:
	CMouse();
	virtual ~CMouse();

	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();
};

#endif // !__MOUSE_H__
