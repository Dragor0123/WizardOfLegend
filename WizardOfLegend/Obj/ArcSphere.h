#pragma once
#include "ArcRel.h"

class CArcSphere : public CArcRel
{
public:
	CArcSphere();
	virtual ~CArcSphere();

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
	CObj*	m_pLastMadeBullet;
	bool	m_bShot;
};

