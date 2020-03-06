#pragma once

#ifndef __ARCDRAGONARC_H__
#define __ARCDRAGONARC_H__

#include "ArcRel.h"

class CArcDragonarc : public CArcRel
{
public:
	CArcDragonarc();
	virtual ~CArcDragonarc();

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
	CObj*	Create_Bullet(bool _bClockwise);
	CObj*	Create_SigBullet(bool _bClockwise, float _fAngle);

private:
	bool	m_bInitFire;
	float	m_fAngle;
	float	m_fBulletTick;
	int		m_iBulletCount;

	static const int iBULLETMAX;
	static const int iSIG_BULLETMAX;
};

#endif