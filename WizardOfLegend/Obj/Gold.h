#pragma once

#ifndef __GOLD_H__
#define __GOLD_H__
#include "MoveObj.h"

class CGold : public CMoveObj
{
public:
	CGold();
	virtual ~CGold();

public:
	virtual int	Get_Collision_Code() const { return CC_GGOLD; }

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	int Get_Gold() { return m_iGold; }

private:
	int		m_iGold;
};

#endif
