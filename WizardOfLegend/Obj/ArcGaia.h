#pragma once
#include "ArcRel.h"

class CArcGaia : public CArcRel
{
public:
	CArcGaia();
	virtual ~CArcGaia();

public:
	// Inherited via CStopObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Release();

public:
	virtual void Fire_Skill();
	virtual void Key_Up_Action();
	// Inherited via CArcRel
	virtual void Passive_Effect();

private:
	CObj*	Create_Shield(float _fAngle);
};

