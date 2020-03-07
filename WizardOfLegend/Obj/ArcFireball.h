#pragma once

#ifndef __ARCFIREBALL_H__
#define __ARCFIREBALL_H__

#include "ArcRel.h"

// code : 2
class CArcFireball : public CArcRel
{
public:
	CArcFireball();
	virtual ~CArcFireball();

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
	CObj*	Create_Bullet();
	CObj*	Create_SigBullet(float _fAngle);

private:
	bool		m_bInitFire;
	float		m_fAngle;
	float		m_fBulletTick;
	int			m_iBulletCount;
	
	static const int iSIG_BULLETMAX;
};

#endif