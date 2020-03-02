#pragma once
#include "StopObj.h"

class CIcicleEffect : public CStopObj
{
public:
	CIcicleEffect();
	virtual ~CIcicleEffect();

public:
	// Inherited via CStopObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

private:
	float m_fLifeTime;
	static const float m_fLifeTLimit;
};

