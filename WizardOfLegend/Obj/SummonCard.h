#pragma once
#include "StopObj.h"

class CObj;

class CSummonCard : public CStopObj
{
public:
	CSummonCard();
	virtual ~CSummonCard();

public:
	// Inherited via CStopObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	virtual void Move_Frame();
	virtual void Scene_Change();

private:
	CObj* Create_Monster();
};

