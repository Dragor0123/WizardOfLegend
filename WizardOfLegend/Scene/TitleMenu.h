#pragma once

#ifndef __TITLEMENU_H__
#define __TITLEMENU_H__

#include "Scene.h"

class CTitleMenu : public CScene
{
public:
	CTitleMenu();
	virtual ~CTitleMenu();

	// Inherited via CScene
	virtual bool Initialize();
	virtual void Input(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();
};

#endif
