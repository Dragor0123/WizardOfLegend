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
	virtual void Key_Check();
	virtual bool Initialize();
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Collision(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime);
	virtual void Release();


private:
	static const int iLogoX;
	static int iLogoY;
	bool m_bTitleLogoUp;
};

#endif
