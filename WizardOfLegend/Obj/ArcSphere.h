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
	virtual void Fire_Skill();
	void Key_Up_Action();
	// Inherited via CArcRel
	virtual void Passive_Effect();

private:
	CObj*	Create_Bullet();
	CObj*	m_pLastMadeBullet;
	bool	m_bShot;
};

