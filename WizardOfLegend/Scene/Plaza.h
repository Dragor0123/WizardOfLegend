#pragma once

#ifndef __PLAZA_H__
#define __PLAZA_H__
#include "Scene.h"

class CPlaza : public CScene
{
public:
	CPlaza();
	virtual ~CPlaza();

	// Inherited via CScene
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Collision(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();
	virtual void Key_Check();
};

#endif