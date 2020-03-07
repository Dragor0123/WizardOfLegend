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
	CObj*	Create_SigBullet(float _fAngle, bool _bMakingIce);
private:
	bool	m_bInitFire;
	float	m_fAngle;
	float	m_fBulletTick;
	int		m_iBulletCount;

	float	m_fSigCycleTick;
	int		m_iSigCycleCount;
	bool	m_bSigCycleTime;

	static const int iBULLETMAX;
	static const int iSIG_CYCLEMAX;
};

#endif