#pragma once

#include "Shield.h"
class CGaiaShield : public CShield
{
public:
	CGaiaShield();
	virtual ~CGaiaShield();

public:
	// Inherited via CMoveObj
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

	void Change_HitRect();

public:
	virtual void Move_Frame();
	virtual void Scene_Change();
};

