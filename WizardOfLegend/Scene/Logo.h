#pragma once

#ifndef __LOGO_H__
#define __LOGO_H__

#include "Scene.h"

class CLogo : public CScene
{
public:
	CLogo();
	virtual ~CLogo();

	// Inherited via CScene
	virtual bool Initialize();
	virtual void Input(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();
};

#endif