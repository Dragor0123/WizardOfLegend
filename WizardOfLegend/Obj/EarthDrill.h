#pragma once

#ifndef __EARTHDRILL_H__
#define __EARTHDRILL_H__

#include "MeeleBullet.h"
class CEarthDrill : public CMeeleBullet
{
public:
	static const float fPLAYER_POSIN_RANGE;

public:
	CEarthDrill();
	virtual ~CEarthDrill();

public:
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	virtual void Move_Frame();
	virtual void Scene_Change();

public:
	virtual int		Get_Collision_Code() const;

protected:
	virtual void	Change_HitRect();

private:
	static const DWORD dwFIRE_FRAMESPEED;
};

#endif