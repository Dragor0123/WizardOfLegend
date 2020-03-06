#pragma once

#ifndef __ARCFROSTFAN_H__
#define __ARCFROSTFAN_H__

#include "ArcRel.h"

class CObj;

class CArcFrostFan : public CArcRel
{
public:
	CArcFrostFan();
	virtual ~CArcFrostFan();

public:
	// Inherited via CStopObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Release();

public:
	// Inherited via CArcRel
	virtual void Fire_Skill();
	virtual void Key_Up_Action();
	virtual void Passive_Effect();

private:
	CObj*	Create_Bullet(float _fAngle);

private:
	bool	m_bInitFire;
	float	m_fAngle;
	float	m_fBulletTick;
	int		m_iBulletCount;
};

#endif